#pragma once
#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>
#include <vector>

#include <assimp/scene.h>
#include "Mesh.hpp"

namespace slaggy
{
	class Shader;

	class Model
	{
	public:
		Model(const std::string& path);
		~Model();
		void draw(const Shader& shader);
	private:
		std::vector<Mesh> _meshes;
		std::string _directory;
		std::vector<Mesh::Texture> _texturesLoaded;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);
		static unsigned int textureFromFile(const std::string& path, const std::string& directory);
	};
}
#endif