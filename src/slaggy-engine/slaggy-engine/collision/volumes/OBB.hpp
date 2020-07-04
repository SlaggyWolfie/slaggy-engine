#pragma once
#ifndef OBB_HPP
#define OBB_HPP

#include <collision/volumes/Box.hpp>

namespace slaggy
{
	class OBB : public Box
	{
	public:
		bool intersects(const IIntersect& intersect) const override;
		bool intersects(const Plane& plane) const override;
		bool intersects(const Sphere& sphere) const override;
		bool intersects(const AABB& aabb) const override;
		bool intersects(const OBB& obb) const override;

		glm::vec3 min() const override;
		glm::vec3 max() const override;
		glm::vec3 unorientedMin() const;
		glm::vec3 unorientedMax() const;
		
		bool isInside(glm::vec3 point) const override;
		glm::vec3 closestPointTo(glm::vec3 point) const override;
	};
}
#endif