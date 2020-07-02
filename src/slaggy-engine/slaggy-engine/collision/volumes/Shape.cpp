#include "Shape.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <core/Transform.hpp>

namespace slaggy
{
	glm::vec3 Shape::center() const
	{
		return transform()->position();
	}

	glm::mat4 Shape::transformationMatrix() const
	{
		return transform()->transformationMatrix();
	}

	glm::mat4 Shape::scaledTransformationMatrix() const
	{
		return glm::scale(transformationMatrix(), glm::vec3(radius()));
	}
}
