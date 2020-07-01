#pragma once
#ifndef BOX_DEBUG_HPP
#define BOX_DEBUG_HPP

#include <memory>

// ReSharper disable once CppUnusedIncludeDirective
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace slaggy
{
	class Box;
	class Shader;
	
	class BoxDebug
	{
	public:
		static BoxDebug& instance();
		
		BoxDebug();
		~BoxDebug();
		
		unsigned vao() const;
		void render(const glm::vec3& color, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) const;
		void render(glm::vec3 color, const Box& box, const glm::mat4& view, const glm::mat4& proj) const;
		
	private:
		static std::unique_ptr<BoxDebug> _instance;		
		
		unsigned _vao = 0;
		unsigned _vbo = 0;

		std::unique_ptr<Shader> _shader;
	};
}
#endif