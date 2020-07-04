#pragma once
#ifndef LOGGING_PROGRAM_NR_HPP
#define LOGGING_PROGRAM_NR_HPP
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
	class AABB;
	class Camera;
	class Entity;
	class Shape;
	class OctreeMovement;

	class LoggingNoRenderingProgram : public Program
	{
	public:
		using color = glm::vec3;
		using color4 = glm::vec4;

		const int INIT_ERROR = -1;
		const int INITIAL_SCREEN_WIDTH = 800;
		const int INITIAL_SCREEN_HEIGHT = 600;

		double deltaTime = 0;
		bool firstMouse = true;

		glm::vec2 lastMousePosition = glm::vec2
		(
			float(INITIAL_SCREEN_WIDTH) / 2,
			float(INITIAL_SCREEN_HEIGHT) / 2
		);

		static void process_exit_input(GLFWwindow* window);

		int run() override;

		void createObject(
			std::vector<std::unique_ptr<Entity>>& objectContainer,
			std::vector<Shape*>& colliderContainer,
			std::vector<OctreeMovement*>& movers,
			const AABB& bounds,
			const glm::vec3& spherePosition, float sphereRadius, float speed, float objectHalfSize) const;
	};
}
#endif