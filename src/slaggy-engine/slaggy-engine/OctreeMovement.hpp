#pragma once
#ifndef SIMPLE_PHYSICS_BEHAVIOR_HPP
#define SIMPLE_PHYSICS_BEHAVIOR_HPP

#include <core/Behavior.hpp>

#include <glm/glm.hpp>
#include "collision/volumes/AABB.hpp"

namespace slaggy
{
	class OctreeMovement : public Behavior, public ICloneableTemplate<OctreeMovement>
	{
	public:
		glm::vec3 velocity = glm::vec3(0);

		void setBounds(AABB* bounds);
		OctreeMovement* clone() override;
		void fixedUpdate() override;
		
	private:
		AABB* _bounds = nullptr;
	};
}
#endif