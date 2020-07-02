#pragma once
#ifndef SPHERE_DEBUG_HPP
#define SPHERE_DEBUG_HPP

#include <memory>

// ReSharper disable once CppUnusedIncludeDirective
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <utils/Icosphere.h>

namespace slaggy
{
	class Sphere;
	class Shader;

	class SphereDebug
	{
	public:
		static SphereDebug& instance();

		SphereDebug();
		~SphereDebug();

		unsigned vao() const;
		void render(const glm::vec3& color, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) const;
		void render(const glm::vec3& color, const Sphere& sphere, const glm::mat4& view, const glm::mat4& proj) const;

		Icosphere sphere;
		
	private:
		static std::unique_ptr<SphereDebug> _instance;

		unsigned _vao = 0;
		unsigned _vbo = 0;
		unsigned _ebo = 0;

		unsigned _indexCount = 0;


		std::unique_ptr<Shader> _shader;
	};
}

#endif