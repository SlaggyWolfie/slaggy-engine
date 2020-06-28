#include "Shape.hpp"

#include <core/Entity.hpp>
#include <core/Transform.hpp>

namespace slaggy
{
	void Shape::setCenterOffset(const glm::vec3 centerOffset)
	{
		_centerOffset = centerOffset;
	}

	glm::vec3 Shape::centerOffset() const
	{
		return _centerOffset;
	}

	glm::vec3 Shape::center() const
	{
		return entity().getBehavior<Transform>()->position() + _centerOffset;
	}

	glm::mat4 Shape::transformationMatrix() const
	{
		return entity().getBehavior<Transform>()->transformationMatrix();
	}
}
