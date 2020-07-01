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
		return pb;
	}

	void OctreeMovement::fixedUpdate()
	{
		entity().transform()->translate(velocity);

		//if ()
	}
}
