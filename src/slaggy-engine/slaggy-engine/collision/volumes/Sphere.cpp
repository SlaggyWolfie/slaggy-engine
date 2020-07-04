#include "Sphere.hpp"

#include <collision/Geometry.hpp>
#include <utils/SphereDebug.hpp>

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

	bool Sphere::intersects(const IIntersect& shape) const
	{
		return shape.intersects(*this);
	}

	bool Sphere::intersects(const Plane& plane) const
	{
		return Geometry::intersects(*this, plane);
	}

	bool Sphere::intersects(const Sphere& sphere) const
	{
		return Geometry::intersects(*this, sphere);
	}

	bool Sphere::intersects(const AABB& aabb) const
	{
		return Geometry::intersects(*this, aabb);
	}

	bool Sphere::intersects(const OBB& obb) const
	{
		return Geometry::intersects(*this, obb);
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

	void Sphere::render(const glm::mat4& view, const glm::mat4& proj) const
	{
		SphereDebug::instance().render(glm::vec3(0, 1, 1), *this, view, proj);
	}

	void Sphere::render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const
	{
		SphereDebug::instance().render(color, *this, view, proj);
	}
	
}
