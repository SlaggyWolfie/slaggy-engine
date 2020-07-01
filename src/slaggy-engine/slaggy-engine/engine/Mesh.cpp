#include "Mesh.hpp"

#include <vector>
#include <utility>

#include <glad/glad.h>

#include <engine/Shader.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slaggy
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		: vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures))
	{
		setup();
	}

	Mesh::~Mesh()
	{
		//glDeleteVertexArrays(1, &_vao);
		//glDeleteBuffers(1, &_vbo);
		//glDeleteBuffers(1, &_ebo);
	}

	void Mesh::setup()
	{
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ebo);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// > vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// uvs
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinate));

		// > vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		// > vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

		glBindVertexArray(0);
	}

	void Mesh::draw(const Shader& shader)
	{
		unsigned int diffuseNumber = 1;
		unsigned int specularNumber = 1;
		unsigned int normalNumber = 1;
		unsigned int heightNumber = 1;

		for (unsigned int i = 0; i < textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			const std::string name = textures[i].type;

			// TODO
			// hard-coded but whatever for now
			if (name == "texture_diffuse") number = std::to_string(diffuseNumber++);
			else if (name == "texture_specular") number = std::to_string(specularNumber++);
			else if (name == "texture_normal") number = std::to_string(normalNumber++);
			else if (name == "texture_height") number = std::to_string(heightNumber++);

			std::string address("material.");
			address.append(name).append(number);
			shader.set(address, (int)i); // no idea why tbh but it works (finally)

			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		// Draw Mesh / Pipe in bound information
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	unsigned Mesh::vao() const
	{
		return _vao;
	}

	void Mesh::freeGL() const
	{
		glDeleteVertexArrays(1, &_vao);
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
	}	
}
