#include "Plane.hpp"

#include <collision/Geometry.hpp>

namespace slaggy
{
	bool Plane::intersects(const IIntersect& shape) const
	{
		return shape.intersects(*this);
	}

	bool Plane::intersects(const Plane& plane) const
	{
		return Geometry::intersects(*this, plane);
	}

	bool Plane::intersects(const Sphere& sphere) const
	{
		return Geometry::intersects(sphere, *this);
	}

	bool Plane::intersects(const AABB& aabb) const
	{
		return Geometry::intersects(aabb, *this);
	}

	bool Plane::intersects(const OBB& obb) const
	{
		return Geometry::intersects(obb, *this);
	}

	bool Plane::on(const glm::vec3 point) const
	{
		const float projection = glm::dot(point, normal);
		return std::abs(projection - distance()) < Geometry::epsilon;
	}

	glm::vec3 Plane::closestPointTo(const glm::vec3 point) const
	{
		const float projection = glm::dot(point, normal);
		const float distanceFromPlane = projection - distance();
		return point - distanceFromPlane * normal;
	}
}
