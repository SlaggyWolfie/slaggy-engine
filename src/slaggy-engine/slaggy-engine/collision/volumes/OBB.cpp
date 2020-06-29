#include "OBB.hpp"

#include <core/Entity.hpp>
#include <core/Transform.hpp>

#include <collision/Geometry.hpp>

namespace slaggy
{
	bool OBB::intersects(Shape& shape)
	{
		return shape.intersects(*this);
	}

	bool OBB::intersects(Sphere& sphere)
	{
		return Geometry::intersection(sphere, *this);
	}

	bool OBB::intersects(AABB& aabb)
	{
		return Geometry::intersection(aabb, *this);
	}

	bool OBB::intersects(OBB& obb)
	{
		return Geometry::intersection(obb, *this);
	}

	// TODO Horror
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

	void OBB::render()
	{
		//glm::mat4 model = getWorldTransform();
		//model = glm::translate(model, getCenterOffset());
		//model = glm::scale(model, _halfSize);

		//Octree::getVisualBox()->render(
		//	glm::vec3(1, 0, 0),
		//	model,
		//	world->getMainCamera()->getView(),
		//	world->getMainCamera()->getProjection());
	}

	Behavior* OBB::clone()
	{
		return nullptr;
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
