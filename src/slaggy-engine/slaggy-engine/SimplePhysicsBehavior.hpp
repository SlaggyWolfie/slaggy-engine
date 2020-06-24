#pragma once
#ifndef SIMPLE_PHYSICS_BEHAVIOR_HPP
#define SIMPLE_PHYSICS_BEHAVIOR_HPP

#include <core/Behavior.hpp>

#include <glm/glm.hpp>

namespace slaggy
{
	class SimplePhysicsBehavior : public Behavior, public ICloneableTemplate<SimplePhysicsBehavior>
	{
	public:
		SimplePhysicsBehavior* clone() override;
		void fixedUpdate() override;
		
		void setConstantVelocity(glm::vec3 velocity);
		
	private:
		
		glm::vec3 _velocity = glm::vec3(0);
	};
}
#endif