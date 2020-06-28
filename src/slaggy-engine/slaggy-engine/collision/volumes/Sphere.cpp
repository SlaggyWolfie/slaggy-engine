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

	bool Sphere::intersects(Shape& shape)
	{
		return shape.intersects(*this);
	}

	bool Sphere::intersects(Sphere& sphere)
	{
		return Geometry::intersection(*this, sphere);
	}

	bool Sphere::intersects(AABB& aabb)
	{
		return Geometry::intersection(*this, aabb);
	}

	bool Sphere::intersects(OBB& obb)
	{
		return Geometry::intersection(*this, obb);
	}

	glm::vec3 Sphere::halfSize() const
	{
		return glm::vec3(_radius);
	}

	glm::vec3 Sphere::size() const
	{
		return halfSize() * 2.0f;
	}

	glm::vec3 Sphere::min() const
	{
		return center() - halfSize();
	}

	glm::vec3 Sphere::max() const
	{
		return center() + halfSize();
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

	void Sphere::render() {	}
}
