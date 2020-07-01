#pragma once
#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>

#include "glm/glm.hpp"

namespace slaggy
{
	class Shader;

	class Mesh
	{
	public:
		struct Vertex
		{
			glm::vec3 position, normal;
			glm::vec2 textureCoordinate;
			glm::vec3 tangent, bitangent;
		};

		struct Texture
		{
			unsigned int id = 0;
			std::string type;
			std::string path;
		};

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		~Mesh();

		void draw(const Shader& shader);
		unsigned int vao() const;
		void freeGL() const;
	private:
		unsigned int _vao = 0, _vbo = 0, _ebo = 0;
		void setup();
	};
}
#endif