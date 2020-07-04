#include "AABB.hpp"

#include <collision/Geometry.hpp>

namespace slaggy
{
	bool AABB::intersects(const IIntersect& intersect) const
	{
		return intersect.intersects(*this);
	}

	bool AABB::intersects(const Plane& plane) const
	{
		return Geometry::intersects(*this, plane);
	}

	bool AABB::intersects(const Sphere& sphere) const
	{
		return Geometry::intersects(sphere, *this);
	}

	bool AABB::intersects(const AABB& aabb) const
	{
		return Geometry::intersects(*this, aabb);
	}

	bool AABB::intersects(const OBB& obb) const
	{
		return Geometry::intersects(*this, obb);
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

	Transform* AABB::transform() const
	{
		return nullptr;
	}
}
