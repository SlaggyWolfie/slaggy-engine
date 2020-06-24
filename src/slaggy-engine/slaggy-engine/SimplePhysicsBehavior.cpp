#include "SimplePhysicsBehavior.hpp"

#include "Entity.hpp"
#include "Transform.hpp"

namespace slaggy
{
	SimplePhysicsBehavior* SimplePhysicsBehavior::clone()
	{
		SimplePhysicsBehavior* pb = new SimplePhysicsBehavior;

		pb->_velocity = _velocity;

		return pb;
	}

	void SimplePhysicsBehavior::fixedUpdate()
	{
		// TODO
		entity().getBehavior<Transform>()->translate(_velocity);
	}

	void SimplePhysicsBehavior::setConstantVelocity(const glm::vec3 velocity)
	{
		_velocity = velocity;
	}
}