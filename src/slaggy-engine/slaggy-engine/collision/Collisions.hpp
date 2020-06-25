#pragma once
#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

namespace slaggy
{
	class Shape;
	class BB;
	class OBB;
	class AABB;
	class Sphere;
	
	class Collisions
	{
	public:
		static bool intersection(Sphere& one, Sphere& other);
		static bool intersection(Sphere& sphere, AABB& aabb);
		static bool intersection(Sphere& sphere, OBB& obb);
		static bool intersection(AABB& one, AABB& other);
		static bool intersection(AABB& aabb, OBB& obb);
		static bool intersection(OBB& one, OBB& other);

	private:
		static bool sphereTest(Shape& lhs, Shape& rhs);
		static bool aabbTest(BB& lhs, BB& rhs);
	};
}

#endif