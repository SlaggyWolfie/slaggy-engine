#include "OctreeMovement.hpp"

#include <core/Entity.hpp>
#include <core/Transform.hpp>

namespace slaggy
{
	void OctreeMovement::setBounds(AABB* bounds)
	{
		_bounds = bounds;
	}

	OctreeMovement* OctreeMovement::clone()
	{
		auto pb = new OctreeMovement;
		pb->velocity = velocity;
		pb->_bounds = _bounds;
		return pb;
	}

	void OctreeMovement::fixedUpdate()
	{
		entity().transform()->translate(velocity);

		// If the object gets out of bounds, reverse its velocity 
		for (unsigned i = 0; i < 3; ++i)
			if (velocity[i] != glm::clamp(velocity[i], _bounds->min()[i], _bounds->max()[i]))
				velocity[i] = -velocity[i];
	}
}
