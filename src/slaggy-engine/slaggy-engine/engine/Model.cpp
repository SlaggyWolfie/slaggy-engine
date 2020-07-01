#include "Model.hpp"

#include <iostream>

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <stb_image.h>

namespace slaggy
{
	Model::Model(const std::string& path)
	{
		loadModel(path);
	}

	Model::~Model()
	{
		for (auto& mesh : _meshes) mesh.freeGL();
		for (auto& texture : _texturesLoaded) glDeleteTextures(1, &texture.id);
	}

	void Model::draw(const Shader& shader)
	{
		for (auto& mesh : _meshes) mesh.draw(shader);
	}

	void Model::loadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}

		_directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// > process all node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			_meshes.push_back(processMesh(mesh, scene));
		}

		// > then process the children
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Mesh::Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Mesh::Texture> textures;

		auto convertVec3 = [](const aiVector3D& aiVec) -> glm::vec3
		{
			return glm::vec3(aiVec.x, aiVec.y, aiVec.z);
		};

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Mesh::Vertex vertex{};
			vertex.position = convertVec3(mesh->mVertices[i]);
			vertex.normal = convertVec3(mesh->mNormals[i]);

			if (mesh->HasTextureCoords(0))
			{
				vertex.textureCoordinate = convertVec3(mesh->mTextureCoords[0][i]);
			}
			else vertex.textureCoordinate = glm::vec2(0);

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.tangent = convertVec3(mesh->mTangents[i]);
				vertex.bitangent = convertVec3(mesh->mBitangents[i]);
			}
			else
			{
				vertex.tangent = vertex.bitangent = glm::vec3(0);
			}

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			const aiFace& face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		// > we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// > as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// > Same applies to other texture as the following list summarizes:
		// > diffuse: texture_diffuseN
		// > specular: texture_specularN
		// > normal: texture_normalN

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(
				material, aiTextureType_DIFFUSE, "texture_diffuse");
			std::vector<Mesh::Texture> specularMaps = loadMaterialTextures(
				material, aiTextureType_SPECULAR, "texture_specular");

			std::vector<Mesh::Texture> normalMaps = loadMaterialTextures(
				material, aiTextureType_NORMALS, "texture_normal");
			std::vector<Mesh::Texture> heightMaps = loadMaterialTextures(
				material, aiTextureType_HEIGHT, "texture_height");

			// I have no idea tbh
			//std::vector<Mesh::Texture> normalMaps = loadMaterialTextures(
			//	material, aiTextureType_HEIGHT, "texture_normal");
			//std::vector<Mesh::Texture> heightMaps = loadMaterialTextures(
			//	material, aiTextureType_AMBIENT, "texture_height");

			std::vector<Mesh::Texture> emissiveMaps = loadMaterialTextures(
				material, aiTextureType_EMISSIVE, "texture_emissive");

			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
			textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial* material, const aiTextureType type, const std::string& typeName)
	{
		std::vector<Mesh::Texture> textures;
		for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
		{
			aiString str;
			material->GetTexture(type, i, &str);

			bool skip = false;

			// avoid loading the same texture and just assign the loaded one
			for (const auto& j : _texturesLoaded)
			{
				if (std::strcmp(j.path.data(), str.C_Str()) != 0) continue;

				textures.push_back(j);
				skip = true;
				break;
			}

			if (skip) continue;

			// > if texture hasn't been loaded already, load it
			Mesh::Texture texture;
			texture.id = textureFromFile(str.C_Str(), _directory);
			texture.type = typeName;
			texture.path = str.C_Str();

			_texturesLoaded.push_back(texture);
			textures.push_back(texture);
		}

		return textures;
	}

	unsigned int Model::textureFromFile(const std::string& path, const std::string& directory)
	{
		std::string filename(directory);
		filename.append("/").append(path);

		unsigned int textureID = 0;
		glGenTextures(1, &textureID);

		int width, height, numberOfComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numberOfComponents, 0);
		if (data)
		{
			GLenum format;
			if (numberOfComponents == 1) format = GL_RED;
			else if (numberOfComponents == 3) format = GL_RGB;
			else if (numberOfComponents == 4) format = GL_RGBA;
			else format = GL_RGB;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			//float borderColor[] = { 1, 1, 0, 1 }; // brown-ish?
			//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
		}

		stbi_image_free(data);

		return textureID;
	}
}