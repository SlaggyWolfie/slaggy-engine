#pragma once
#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include <glm/glm.hpp>

namespace slaggy
{
	class Plane;
	class Shape;
	class Box;
	class OBB;
	class AABB;
	class Sphere;
	
	class Geometry
	{
	public:
		static float epsilon;
		
		static bool intersects(const Plane& lhs, const Plane& rhs);
		static bool intersects(const Sphere& one, const Sphere& other);
		static bool intersects(const Sphere& sphere, const Plane& plane);
		static bool intersects(const Sphere& sphere, const AABB& aabb);
		static bool intersects(const Sphere& sphere, const OBB& obb);
		static bool intersects(const AABB& lhs, const AABB& rhs);
		static bool intersects(const AABB& aabb, const Plane& plane);
		static bool intersects(const AABB& aabb, const OBB& obb);
		static bool intersects(const OBB& lhs, const OBB& rhs);
		static bool intersects(const OBB& obb, const Plane& plane);

		static bool sphereTest(const Shape& lhs, const Shape& rhs);
		static bool aabbTest(const AABB& lhs, const AABB& rhs);
		static bool satTest(const Box& lhs, const Box& rhs);

		static float distanceSqr(const glm::vec3& lhs, const glm::vec3& rhs);
		static float magnitudeSqr(const glm::vec3& v);
		static glm::vec3 fabsf(const glm::vec3& v);

		//static void reflectVelocity(glm::vec3& lhs, glm::vec3& rhs);
		static void reflectVelocity(const glm::vec3& centerLhs, const glm::vec3& centerRhs, glm::vec3& velocityLhs, glm::vec3& velocityRhs);
	};
}

#endif