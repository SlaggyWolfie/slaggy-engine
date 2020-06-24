#pragma once
#ifndef PHYSICS_BEHAVIOR_HPP
#define PHYSICS_BEHAVIOR_HPP
#include "Behavior.hpp"

#include <glm/glm.hpp>

namespace slaggy
{
	class PhysicsBehavior : public Behavior, public ICloneableTemplate<PhysicsBehavior>
	{
	public:
		PhysicsBehavior* clone() override;
		void fixedUpdate() override;
		
		void setConstantForce(glm::vec3 force);
		
	private:
		glm::vec3 _acceleration = glm::vec3(0);
		
		glm::vec3 _force = glm::vec3(0);
		float _mass = 1;
		
		glm::vec3 _velocity = glm::vec3(0);
	};
}
#endif