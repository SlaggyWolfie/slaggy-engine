#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

namespace slaggy
{
	std::map<Shader::ShaderType, Shader::ShaderTypeInfo> Shader::_shaderTypes =
	{
		{ShaderType::VERTEX, {"VERTEX", GL_VERTEX_SHADER}},
		{ShaderType::FRAGMENT, {"FRAGMENT", GL_FRAGMENT_SHADER}},
		{ShaderType::GEOMETRY, {"GEOMETRY", GL_GEOMETRY_SHADER}}
	};

	std::map<unsigned, unsigned> Shader::_idCounter;

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vCode, fCode;

		read(vertexPath, vCode);
		read(fragmentPath, fCode);

		const unsigned int vID = compile(vCode, ShaderType::VERTEX);
		const unsigned int fID = compile(fCode, ShaderType::FRAGMENT);

		id = link({ vID, fID });

		incrementCounter();
	}

	Shader::Shader(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath)
	{
		std::string vCode, fCode, gCode;

		read(vertexPath, vCode);
		read(fragmentPath, fCode);
		read(geometryPath, gCode);

		const unsigned int vID = compile(vCode, ShaderType::VERTEX);
		const unsigned int fID = compile(fCode, ShaderType::FRAGMENT);
		const unsigned int gID = compile(gCode, ShaderType::GEOMETRY);

		id = link({ vID, fID, gID });

		incrementCounter();
	}

	Shader::Shader(const std::string& path) :
		Shader
		(
			std::string(path).append(".vert"),
			std::string(path).append(".frag")
		)
	{ }

	Shader::~Shader()
	{
		decrementCounter();
	}

	void Shader::idReassign(const Shader& other)
	{
		decrementCounter();
		id = other.id;
		incrementCounter();
	}

	Shader::Shader(const Shader& other)
	{
		idReassign(other);
	}

	Shader::Shader(Shader&& other) noexcept
	{
		idReassign(other);
	}

	Shader& Shader::operator=(const Shader& other)
	{
		if (this == &other) return *this;

		idReassign(other);
		return *this;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		idReassign(other);
		return *this;
	}

	void Shader::read(const std::string& path, std::string& code)
	{
		std::ifstream file;

		// > ensure ifstream objects can throw exceptions
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		//-----OPEN & READ-----//
		// Try to open and read the shader files
		// and place them into memory
		// ALTERNATIVE METHOD in MGE: read line-by-line and add it to a string
		try
		{
			// opening the files
			file.open(path);

			// setting up streams (reading pipelines ?)
			std::stringstream stream;

			// > read file's buffer contents into the stream
			stream << file.rdbuf();

			// close
			file.close();

			// place into memory
			code = stream.str();
		}
		catch (std::ifstream::failure& exception)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_FOUND_OR_READ\n" << exception.what() << std::endl;
			// TODO fix shader failure not stopping the rest of the process
			//return;
		}
	}

	unsigned Shader::compile(const std::string& code, const ShaderType shaderType)
	{
		const char* code_c = code.c_str();
		const ShaderTypeInfo& info = _shaderTypes[shaderType];

		//-----COMPILE & LINK-----//
		unsigned int id = 0;
		int success = -1;
		char log[512];

		// Vertex Shader
		id = glCreateShader(info.glID);
		glShaderSource(id, 1, &code_c, nullptr);
		glCompileShader(id);

		// print compilation errors
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, nullptr, log);
			std::cout << "ERROR::SHADER::" << info.name << "::COMPILATION_FAILED\n" << log << std::endl;
		}

		return id;
	}

	unsigned Shader::link(const std::vector<unsigned>& shaderIds)
	{
		int success = -1;
		char log[512];

		const unsigned int programID = glCreateProgram();

		for (unsigned id : shaderIds) glAttachShader(programID, id);

		glLinkProgram(programID);

		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programID, 512, nullptr, log);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
		}

		for (unsigned id : shaderIds) glDeleteShader(id);

		return programID;
	}

	void Shader::use() const
	{
		glUseProgram(id);
	}

	void Shader::set(const std::string& name, const bool value) const
	{
		glUniform1i(location(name), static_cast<int>(value));
	}

	void Shader::set(const std::string& name, const int value) const
	{
		glUniform1i(location(name), value);
	}

	void Shader::set(const std::string& name, const unsigned int value) const
	{
		glUniform1ui(location(name), value);
	}

	void Shader::set(const std::string& name, const float value) const
	{
		glUniform1f(location(name), value);
	}

	void Shader::set(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(location(name), 1, glm::value_ptr(value));
	}

	void Shader::set(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(location(name), 1, glm::value_ptr(value));
	}

	void Shader::set(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(location(name), 1, glm::value_ptr(value));
	}

	void Shader::set(const std::string& name, const glm::mat3& value) const
	{
		glUniformMatrix3fv(location(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::set(const std::string& name, const glm::mat4& value) const
	{
		glUniformMatrix4fv(location(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	unsigned Shader::location(const std::string& name) const
	{
		return glGetUniformLocation(id, name.c_str());
	}

	void Shader::incrementCounter() const
	{
		_idCounter[id]++;
	}

	void Shader::decrementCounter() const
	{
		_idCounter[id]--;
		if (_idCounter[id] == 0)
			glDeleteProgram(id);
	}
}