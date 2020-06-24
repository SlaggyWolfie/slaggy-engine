#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

void slaggy::Transform::setPosition(const glm::vec3& position)
{
	_transformationMatrix[3] = glm::vec4(position, 1);
}

glm::vec3 slaggy::Transform::position()
{
	return glm::vec3(_transformationMatrix[3]);
}

void slaggy::Transform::setTransformationMatrix(const glm::mat4& matrix)
{
	_transformationMatrix = matrix;
}

glm::mat4 slaggy::Transform::transformationMatrix() const
{
	return _transformationMatrix;
}

glm::vec3 slaggy::Transform::worldForward()
{
	return glm::vec3(0, 0, -1);
}

glm::vec3 slaggy::Transform::worldUp()
{
	return glm::vec3(0, 1, 0);
}

glm::vec3 slaggy::Transform::worldRight()
{
	return glm::vec3(1, 0, 0);
}

glm::vec3 slaggy::Transform::forward()
{
	// we make it negative because it's a left hand matrix
	// while OpenGL operates in right-hand
	// or something dumb
	// TODO investigate
	return -glm::normalize(_transformationMatrix[2]);
}

glm::vec3 slaggy::Transform::up()
{
	return glm::normalize(_transformationMatrix[1]);
}

glm::vec3 slaggy::Transform::right()
{
	return glm::normalize(_transformationMatrix[0]);
}

glm::vec3 slaggy::Transform::transformPoint(const glm::vec3& point) const
{
	return _transformationMatrix * glm::vec4(point, 1);
}

glm::vec3 slaggy::Transform::inverseTransformPoint(const glm::vec3& point) const
{
	return glm::inverse(_transformationMatrix) * glm::vec4(point, 1);
}

glm::vec3 slaggy::Transform::transformDirection(const glm::vec3& direction) const
{
	return _transformationMatrix * glm::vec4(direction, 0);
}

glm::vec3 slaggy::Transform::inverseTransformDirection(const glm::vec3& direction) const
{
	return glm::inverse(_transformationMatrix) * glm::vec4(direction, 0);
}

void slaggy::Transform::translate(const glm::vec3& translation)
{
	_transformationMatrix = glm::translate(_transformationMatrix, translation);
}

slaggy::Transform* slaggy::Transform::clone()
{
	auto transform = new Transform();
	transform->_transformationMatrix = _transformationMatrix;
	return transform;
}
