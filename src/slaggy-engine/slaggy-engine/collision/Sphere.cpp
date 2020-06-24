#include "Sphere.hpp"

#include <collision/Collisions.hpp>

namespace slaggy
{
	void Sphere::setRadius(const float radius)
	{
		_radius = radius;
	}

	float Sphere::radius()
	{
		return _radius;
	}

	bool Sphere::intersects(Shape& shape)
	{
		//std::cout << "Intersection between Sphere and Shape" << std::endl;
		return shape.intersects(*this);
	}

	bool Sphere::intersects(Sphere& sphere)
	{
		//std::cout << "Intersection between Sphere and Sphere" << std::endl;
		return Collisions::intersection(*this, sphere);
	}

	bool Sphere::intersects(AABB& aabb)
	{
		//std::cout << "Intersection between Sphere and AABB" << std::endl;
		return Collisions::intersection(*this, aabb);
	}

	bool Sphere::intersects(OBB& obb)
	{
		//std::cout << "Intersection between Sphere and OBB" << std::endl;
		return Collisions::intersection(*this, obb);
	}

	glm::vec3 Sphere::halfSize()
	{
		return glm::vec3(_radius, _radius, _radius);
	}

	glm::vec3 Sphere::size()
	{
		return halfSize() * 2.0f;
	}

	glm::vec3 Sphere::min()
	{
		return center() - halfSize();
	}

	glm::vec3 Sphere::max()
	{
		return center() + halfSize();
	}

	void Sphere::render() {	}

}
