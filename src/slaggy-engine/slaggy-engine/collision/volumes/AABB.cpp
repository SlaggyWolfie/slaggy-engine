#include "AABB.hpp"

#include <collision/Geometry.hpp>

namespace slaggy
{
	bool AABB::intersects(const Shape& shape) const
	{
		return shape.intersects(*this);
	}

	bool AABB::intersects(const Sphere& sphere) const
	{
		return Geometry::intersection(sphere, *this);
	}

	bool AABB::intersects(const AABB& aabb) const
	{
		return Geometry::intersection(*this, aabb);
	}

	bool AABB::intersects(const OBB& obb) const
	{
		return Geometry::intersection(*this, obb);
	}

	bool AABB::isInside(const glm::vec3 point) const
	{
		const glm::vec3 lmin = min();
		const glm::vec3 lmax = max();

		return lmin.x < point.x && point.x < lmax.x
			&& lmin.y < point.y && point.y < lmax.y
			&& lmin.z < point.z && point.z < lmax.z;
	}

	glm::vec3 AABB::closestPointTo(const glm::vec3 point) const
	{
		return glm::clamp(point, min(), max());
	}
}
