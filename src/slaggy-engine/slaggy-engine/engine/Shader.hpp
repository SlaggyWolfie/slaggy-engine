#pragma once
#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

namespace slaggy
{
	class Shader
	{
		enum class ShaderType { VERTEX, FRAGMENT, GEOMETRY };
		struct ShaderTypeInfo
		{
			std::string name = "";
			GLuint glID = 0;
		};
	
	public:
		unsigned int id = 0;

		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		Shader(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);
		explicit Shader(const std::string& path);
		~Shader();

		Shader(const Shader& other);
		Shader(Shader&& other) noexcept;

		Shader& operator= (const Shader& other);
		Shader& operator= (Shader&& other) noexcept;

		void use() const;

		void set(const std::string& name, bool value) const;
		void set(const std::string& name, int value) const;
		void set(const std::string& name, unsigned int value) const;
		void set(const std::string& name, float value) const;
		void set(const std::string& name, const glm::vec2& value) const;
		void set(const std::string& name, const glm::vec3& value) const;
		void set(const std::string& name, const glm::vec4& value) const;
		void set(const std::string& name, const glm::mat3& value) const;
		void set(const std::string& name, const glm::mat4& value) const;
	
	private:
		static std::map<ShaderType, ShaderTypeInfo> _shaderTypes;	
		static std::map<unsigned int, unsigned int> _idCounter;
		
		void incrementCounter() const;
		void decrementCounter() const;
		void idReassign(const Shader& other);
	
		unsigned int location(const std::string& name) const;
	
		//static void read(const std::string& vertexPath, const std::string& fragmentPath, std::string& vCode, std::string& fCode);
		static void read(const std::string& path, std::string& code);
		//static unsigned int compileAndLink(const std::string& vCode, const std::string& fCode);
		static unsigned int compile(const std::string& code, ShaderType shaderType);
		static unsigned int link(const std::vector<unsigned int>& shaderIds);
	};
}
#endif