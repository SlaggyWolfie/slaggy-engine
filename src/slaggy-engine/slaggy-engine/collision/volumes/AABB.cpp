#include "AABB.hpp"

#include <collision/Geometry.hpp>

namespace slaggy
{
	bool AABB::intersects(Shape& shape)
	{
		return shape.intersects(*this);
	}

	bool AABB::intersects(Sphere& sphere)
	{
		return Geometry::intersection(sphere, *this);
	}

	bool AABB::intersects(AABB& aabb)
	{
		return Geometry::intersection(*this, aabb);
	}

	bool AABB::intersects(OBB& obb)
	{
		return Geometry::intersection(*this, obb);
	}

	void AABB::render()
	{
		// TODO
		//glm::mat4 model = glm::mat4(1);
		//model = glm::translate(model, center());
		//model = glm::scale(model, _halfSize);

		//Octree::getVisualBox()->render(
		//	glm::vec3(0, 1, 0), 
		//	model, 
		//	world->getMainCamera()->getView(),
		//	world->getMainCamera()->getProjection());
	}

	Behavior* AABB::clone()
	{
		return nullptr;
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
