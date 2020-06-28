#pragma once
#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include <glm/glm.hpp>

namespace slaggy
{
	class Shape;
	class Box;
	class OBB;
	class AABB;
	class Sphere;
	
	class Geometry
	{
	public:
		static bool intersection(const Sphere& one, const Sphere& other);
		static bool intersection(const Sphere& sphere, const AABB& aabb);
		static bool intersection(const Sphere& sphere, const OBB& obb);
		static bool intersection(const AABB& lhs, const AABB& rhs);
		static bool intersection(const AABB& lhs, const OBB& rhs);
		static bool intersection(const OBB& lhs, const OBB& rhs);

		static bool sphereTest(const Shape& lhs, const Shape& rhs);
		static bool aabbTest(const AABB& lhs, const AABB& rhs);
		static bool satTest(const Box& lhs, const Box& rhs);

		static float distanceSqr(glm::vec3 lhs, glm::vec3 rhs);
		static float magnitudeSqr(glm::vec3 v);
	};
}

#endif