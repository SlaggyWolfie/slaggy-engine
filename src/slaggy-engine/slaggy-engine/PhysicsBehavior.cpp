#include "PhysicsBehavior.hpp"

#include "Entity.hpp"
#include "Transform.hpp"

namespace slaggy
{
	PhysicsBehavior* PhysicsBehavior::clone()
	{
		PhysicsBehavior* pb = new PhysicsBehavior;

		pb->_acceleration = _acceleration;
		pb->_velocity = _velocity;
		pb->_mass = _mass;
		pb->_force = _force;

		return pb;
	}

	void PhysicsBehavior::fixedUpdate()
	{
		_velocity += _acceleration;

		// TODO
		entity().getBehavior<Transform>()->translate(_velocity);
	}

	void PhysicsBehavior::setConstantForce(const glm::vec3 force)
	{
		_force = force;
		_acceleration = force / _mass;
	}
}
