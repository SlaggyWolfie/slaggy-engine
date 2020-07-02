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

		void setBounds(const Shape& shape, const AABB& outerBounds);
		OctreeMovement* clone() override;
		void fixedUpdate() override;
		
	private:
		glm::vec3 _outerBoundsMin = glm::vec3(0);
		glm::vec3 _outerBoundsMax = glm::vec3(0);

		float _innerRadius = 0;
	};
}
#endif