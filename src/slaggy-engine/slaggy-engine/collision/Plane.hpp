#pragma once
#ifndef PLANE_HPP
#define PLANE_HPP

#include <collision/IIntersect.hpp>

#include <glm/glm.hpp>

namespace slaggy
{
	class Plane : public IIntersect
	{
	public:
		virtual float distance() const = 0;
		glm::vec3 normal = glm::vec3(0);
		
		virtual ~Plane() = default;				

		bool intersects(const IIntersect& shape) const override;
		bool intersects(const Plane& plane) const override;
		bool intersects(const Sphere& sphere) const override;
		bool intersects(const AABB& aabb) const override;
		bool intersects(const OBB& obb) const override;
		
		virtual bool on(glm::vec3 point) const;
		virtual glm::vec3 closestPointTo(glm::vec3 point) const;
	};
}
#endif