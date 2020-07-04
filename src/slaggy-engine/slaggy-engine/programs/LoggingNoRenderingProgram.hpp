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