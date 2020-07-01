#include "OBB.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <collision/Geometry.hpp>

namespace slaggy
{
	bool OBB::intersects(const Shape& shape) const
	{
		return shape.intersects(*this);
	}

	bool OBB::intersects(const Sphere& sphere) const
	{
		return Geometry::intersection(sphere, *this);
	}

	bool OBB::intersects(const AABB& aabb) const
	{
		return Geometry::intersection(aabb, *this);
	}

	bool OBB::intersects(const OBB& obb) const
	{
		return Geometry::intersection(obb, *this);
	}

	glm::vec3 OBB::min() const
	{
		return glm::vec3(transformationMatrix() * glm::vec4(-halfSize(), 1));
	}

	glm::vec3 OBB::max() const
	{
		return glm::vec3(transformationMatrix() * glm::vec4(halfSize(), 1));
	}

	glm::vec3 OBB::unorientedMin() const
	{
		return Box::min();
	}

	glm::vec3 OBB::unorientedMax() const
	{
		return Box::max();
	}
	
	bool OBB::isInside(const glm::vec3 point) const
	{
		const glm::vec3 direction = point - center();
		const glm::mat4 transform = transformationMatrix();
		const glm::vec3 size_ = size();

		for (unsigned i = 0; i < 3; ++i)
		{
			const glm::vec3 axis = glm::vec3(transform[i]);
			const float distance = glm::dot(direction, axis);

			if (distance > size_[i] || distance < -size_[i]) return false;
		}

		return true;
	}

	glm::vec3 OBB::closestPointTo(glm::vec3 point) const
	{
		const glm::vec3 direction = point - center();
		const glm::mat4 transform = transformationMatrix();
		const glm::vec3 size_ = size();

		for (unsigned i = 0; i < 3; ++i)
		{
			const glm::vec3 axis = glm::vec3(transform[i]);
			float distance = glm::dot(direction, axis);

			distance = glm::clamp(distance, -size_[i], size_[i]);

			point += axis * distance;
		}

		return point;
	}
}
