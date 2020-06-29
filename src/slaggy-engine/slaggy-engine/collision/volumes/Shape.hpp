#pragma once
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glm/glm.hpp>
#include <core/Behavior.hpp>

namespace slaggy
{
	class Sphere;
	class AABB;
	class OBB;

	class Shape : public Behavior
	{		
	public:
		virtual ~Shape() = default;
		
		virtual bool intersects(Shape& shape) = 0;
		virtual bool intersects(Sphere& sphere) = 0;
		virtual bool intersects(AABB& aabb) = 0;
		virtual bool intersects(OBB& obb) = 0;
		
		virtual glm::vec3 center() const;

		virtual float radius() const = 0;

		virtual bool isInside(glm::vec3 point) const = 0;
		virtual glm::vec3 closestPointTo(glm::vec3 point) const = 0;

		virtual void render() = 0;
		
		glm::mat4 transformationMatrix() const;
	};
}
#endif
