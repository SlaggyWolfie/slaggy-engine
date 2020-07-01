#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace slaggy
{
	void Transform::setPosition(const glm::vec3& position)
	{
		_transformationMatrix[3] = glm::vec4(position, 1);
	}

	glm::vec3 Transform::position()
	{
		return glm::vec3(_transformationMatrix[3]);
	}

	void Transform::setTransformationMatrix(const glm::mat4& matrix)
	{
		_transformationMatrix = matrix;
	}

	glm::mat4 Transform::transformationMatrix() const
	{
		return _transformationMatrix;
	}

	glm::vec3 Transform::worldForward()
	{
		return glm::vec3(0, 0, -1);
	}

	glm::vec3 Transform::worldUp()
	{
		return glm::vec3(0, 1, 0);
	}

	glm::vec3 Transform::worldRight()
	{
		return glm::vec3(1, 0, 0);
	}

	glm::vec3 Transform::forward()
	{
		// we make it negative because it's a left hand matrix
		// while OpenGL operates in right-hand
		// or something dumb
		// TODO investigate
		return -normalize(_transformationMatrix[2]);
	}

	glm::vec3 Transform::up()
	{
		return normalize(_transformationMatrix[1]);
	}

	glm::vec3 Transform::right()
	{
		return normalize(_transformationMatrix[0]);
	}

	glm::vec3 Transform::transformPoint(const glm::vec3& point) const
	{
		return _transformationMatrix * glm::vec4(point, 1);
	}

	glm::vec3 Transform::inverseTransformPoint(const glm::vec3& point) const
	{
		return inverse(_transformationMatrix) * glm::vec4(point, 1);
	}

	glm::vec3 Transform::transformDirection(const glm::vec3& direction) const
	{
		return _transformationMatrix * glm::vec4(direction, 0);
	}

	glm::vec3 Transform::inverseTransformDirection(const glm::vec3& direction) const
	{
		return inverse(_transformationMatrix) * glm::vec4(direction, 0);
	}

	void Transform::translate(const glm::vec3& translation)
	{
		_transformationMatrix = glm::translate(_transformationMatrix, translation);
	}

	void Transform::rotate(const glm::vec3& axis, const float angleRotation)
	{
		_transformationMatrix = glm::rotate(_transformationMatrix, angleRotation, axis);
	}

	void Transform::scale(const glm::vec3& v)
	{
		_transformationMatrix = glm::scale(_transformationMatrix, v);
	}

	Transform* Transform::clone()
	{
		auto transform = new Transform();
		transform->_transformationMatrix = _transformationMatrix;
		return transform;
	}
	
}
