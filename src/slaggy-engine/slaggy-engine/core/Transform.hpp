#pragma once
#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <core/Behavior.hpp>

namespace slaggy
{
	class Transform : public Behavior, ICloneableTemplate<Transform>
	{
	public:
		void setPosition(const glm::vec3& position);
		glm::vec3 position();

		//void setRotation(const glm::quat& rotation);
		//glm::quat rotation();
		//
		//void setScale(const glm::vec3& scale);
		//glm::vec3 getScale();

		void setTransformationMatrix(const glm::mat4& matrix);
		glm::mat4 transformationMatrix() const;

		static glm::vec3 worldForward();
		static glm::vec3 worldUp();
		static glm::vec3 worldRight();

		glm::vec3 forward();
		glm::vec3 up();
		glm::vec3 right();

		glm::vec3 transformPoint(const glm::vec3& point) const;
		glm::vec3 inverseTransformPoint(const glm::vec3& point) const;
		glm::vec3 transformDirection(const glm::vec3& direction) const;
		glm::vec3 inverseTransformDirection(const glm::vec3& direction) const;

		//void lookAt(Transform* lookAtTarget, const glm::vec3& up = glm::vec3(0, 1, 0));
		void translate(const glm::vec3& translation);
		//void rotate(const glm::vec3& axis, float angleRotation);
		//void rotate(const glm::vec3& eulerAngles);
		//void rotateAround(const glm::vec3& axis, float angleRotation, const glm::vec3& point);
		//void rotate(const glm::vec3& eulerAngles, Space space = World);
		//void scale(const glm::vec3& scaler);
		//void scaleWithPositions(const glm::vec3& scale);

		//void makeStatic();
		//void unmakeStatic();

		Transform* clone() override;
	private:

		//bool _static = false;
		glm::mat4 _transformationMatrix = glm::mat4(1);

		//glm::vec3 _worldPosition = glm::vec3(0);
		//glm::quat _worldRotation = glm::quat();
		//glm::vec3 _worldScale = glm::vec3(1);
		//glm::mat4 _worldMatrix = glm::mat4(1);
		//glm::mat3 _normalMatrix = glm::mat4(1);

		//void _makeLocalMatrixDirty();
		//void _makeWorldMatrixDirty();
		//bool _isLocalMatrixDirty = true;
		//bool _isWorldMatrixDirty = true;

		//glm::mat4 _calculateLocalMatrix() const;
		//glm::mat3 _calculateNormalMatrix() const;

		//glm::vec3 _calculateWorldPosition();
		//glm::quat _calculateWorldRotation();
		//glm::vec3 _calculateWorldScale();
		//glm::mat4 _calculateWorldMatrix();

		////Convenience
		//static glm::vec3 _getTranslation(const glm::mat4& matrix);
		//static glm::quat _getOrientation(const glm::mat4& matrix);
		//static glm::vec3 _getScale(const glm::mat4& matrix);
		//static glm::vec3 _getSkew(const glm::mat4& matrix);
		//static glm::vec4 _getPerspective(const glm::mat4& matrix);
		//static void _getTRS(const glm::mat4& matrix, glm::vec3& position, glm::quat& rotation, glm::vec3& scale);

		//glm::vec4 _transformVector(glm::vec4 vector);
		//glm::vec4 _inverseTransformVector(glm::vec4 vector);

		//glm::vec3 _getLocalSpaceDirection(const glm::vec3& direction);
	};
}
#endif