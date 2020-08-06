#pragma once

#ifndef AABB_HPP
#define AABB_HPP

#include <physics/volumes/Box.hpp>

namespace slaggy
{
	class AABB : public Box
	{
	public:
		bool intersects(const IIntersect& intersect) const override;
		bool intersects(const Plane& plane) const override;
		bool intersects(const Sphere& sphere) const override;
		bool intersects(const AABB& aabb) const override;
		bool intersects(const OBB& obb) const override;
		
		bool isInside(glm::vec3 point) const override;
		glm::vec3 closestPointTo(glm::vec3 point) const override;
		
	protected:
		Transform* transform() const override;
	};
}
#endif //AABB_HPP