#include "Shape.hpp"

#include <core/Entity.hpp>
#include <core/Transform.hpp>

namespace slaggy
{
	glm::vec3 Shape::center() const
	{
		return entity().getBehavior<Transform>()->position();
	}

	glm::mat4 Shape::transformationMatrix() const
	{
		return entity().getBehavior<Transform>()->transformationMatrix();
	}
}
