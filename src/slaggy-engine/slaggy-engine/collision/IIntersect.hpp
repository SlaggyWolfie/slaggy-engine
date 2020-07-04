#pragma once
#ifndef I_INTERSECT_HPP
#define I_INTERSECT_HPP

namespace slaggy
{
	class Plane;
	class Sphere;
	class AABB;
	class OBB;
	
	class IIntersect
	{
	public:
		virtual ~IIntersect() = default;

		virtual bool intersects(const IIntersect& shape) const = 0;
		virtual bool intersects(const Plane& plane) const = 0;
		virtual bool intersects(const Sphere& sphere) const = 0;
		virtual bool intersects(const AABB& aabb) const = 0;
		virtual bool intersects(const OBB& obb) const = 0;
	};
}
#endif