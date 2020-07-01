#pragma once
#ifndef OCTREE_PROGRAM_HPP
#define OCTREE_PROGRAM_HPP
#include "Program.hpp"

#include <string>
#include <stdexcept>
#include <memory>

// ReSharper disable once CppUnusedIncludeDirective
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>

namespace slaggy
{
	class Camera;
	class Entity;
	class Shape;

	class OctreeProgram : public Program
	{
	public:
		using color = glm::vec3;
		using color4 = glm::vec4;

		const int INIT_ERROR = -1;
		const int INITIAL_SCREEN_WIDTH = 800;
		const int INITIAL_SCREEN_HEIGHT = 600;

		// green-ish color
		const color4 _defaultClearColor{ 0.1f, 0.1f, 0.1f, 1.0f };
		color4 _clearColor = _defaultClearColor;

		float mix_ratio = 0.2f;
		float deltaTime = 0;
		float lastFrame = 0;
		bool firstMouse = true;

		glm::vec2 lastMousePosition = glm::vec2
		(
			float(INITIAL_SCREEN_WIDTH) / 2,
			float(INITIAL_SCREEN_HEIGHT) / 2
		);

		Camera* camera = nullptr;

		void framebuffer_size_callback(GLFWwindow* window, int width, int height) const;
		void process_input(GLFWwindow* window);
		void mouse_callback(GLFWwindow*, double x, double y);
		void scroll_callback(GLFWwindow*, double, double yOffset) const;

		// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
		template<typename ... Args>
		std::string string_format(const std::string& format, Args ... args)
		{
			const size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
			if (size <= 0) { throw std::runtime_error("Error during formatting."); }
			const std::unique_ptr<char[]> buf(new char[size]);
			snprintf(buf.get(), size, format.c_str(), args ...);
			return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
		}

		int run() override;

		void createObject(
			std::vector<std::unique_ptr<Entity>>& objectContainer,
			std::vector<Shape*>& colliderContainer,
			const glm::vec3& spherePosition, float sphereRadius, float speed) const;
	};
}
#endif EMPTY_PROGRAM_HPP