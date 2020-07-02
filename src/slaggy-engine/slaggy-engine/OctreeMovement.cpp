#include "OctreeMovement.hpp"

#include <core/Entity.hpp>
#include <core/Transform.hpp>

namespace slaggy
{
	void OctreeMovement::setBounds(const Shape& shape, const AABB& outerBounds)
	{
		_outerBoundsMin = outerBounds.min();
		_outerBoundsMax = outerBounds.max();

		_innerRadius = shape.radius();
	}

	OctreeMovement* OctreeMovement::clone()
	{
		auto pb = new OctreeMovement;

		pb->velocity = velocity;
		pb->_outerBoundsMin = _outerBoundsMin;
		pb->_outerBoundsMax = _outerBoundsMax;
		pb->_innerRadius = _innerRadius;

		return pb;
	}

	void OctreeMovement::fixedUpdate()
	{
		Transform& tf = *entity().transform();
		glm::vec3 position = tf.position();
		position += velocity;

		// If the object gets out of bounds, reverse its velocity 
		for (unsigned i = 0; i < 3; ++i)
		{
			const float min = _outerBoundsMin[i] + _innerRadius;
			const float max = _outerBoundsMax[i] - _innerRadius;

			float& iposition = position[i];
			const float clamp = glm::clamp(iposition, min, max);
			if (iposition < min || iposition > max)
			{
				velocity[i] = -velocity[i];
				iposition = clamp;
			}
		}

		tf.setPosition(position);
	}
}
