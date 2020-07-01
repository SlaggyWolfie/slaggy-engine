#include "Sphere.hpp"

#include <collision/Geometry.hpp>

namespace slaggy
{
	void Sphere::setRadius(const float radius)
	{
		_radius = radius;
	}

	float Sphere::radius() const
	{
		return _radius;
	}

	bool Sphere::intersects(const Shape& shape) const
	{
		return shape.intersects(*this);
	}

	bool Sphere::intersects(const Sphere& sphere) const
	{
		return Geometry::intersection(*this, sphere);
	}

	bool Sphere::intersects(const AABB& aabb) const
	{
		return Geometry::intersection(*this, aabb);
	}

	bool Sphere::intersects(const OBB& obb) const
	{
		return Geometry::intersection(*this, obb);
	}

	bool Sphere::isInside(const glm::vec3 point) const
	{
		return Geometry::distanceSqr(center(), point) < radius() * radius();
	}

	glm::vec3 Sphere::closestPointTo(const glm::vec3 point) const
	{
		const glm::vec3 position(center());
		return position + glm::normalize(point - position) * radius();
	}

	void Sphere::render(const glm::mat4& view, const glm::mat4& proj) const {	}
}
