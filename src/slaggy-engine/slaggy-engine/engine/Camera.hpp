#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace slaggy
{
	class Camera
	{
	private:
		// Global values, placed here for convenience
		static const glm::vec3 ZERO;
		static const glm::vec3 WORLD_UP;
		static const glm::vec3 WORLD_FORWARD;

		// Default camera values
		static const float DEFAULT_YAW;
		static const float DEFAULT_PITCH;
		static const float DEFAULT_SPEED;
		static const float DEFAULT_SENSITIVITY;
		static const float DEFAULT_FOV;

		void updateFromYawPitch();
		void updateNonForward();
		static glm::mat4 bootlegGetLookAtMatrix(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);

	public:
		enum  movement { FORWARD = 1, BACKWARD = 2, LEFT = 4, RIGHT = 8 };

		glm::vec3 forward, up, right;
		glm::vec3 position;

		float yaw, pitch, roll; // y, x, z

		float fov;
		float& zoom = fov;

		float movementSpeed, orientationSensitivity;

		Camera(glm::vec3 position = ZERO, glm::vec3 up = WORLD_UP, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH);

		glm::mat4 viewMatrix() const;
		void lookAt(const glm::vec3& target);
		void updateForward();

		void processMovement(movement directions, bool fast = false, float deltaTime = 1.0f / 60.0f, GLboolean stuck = false);
		void processOrientation(glm::vec2 offset, GLboolean constrain = true);
		void processZoom(float offset);
	};
}
#endif