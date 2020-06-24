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
	protected:
		glm::vec3 _centerOffset = glm::vec3(0);
	public:
		virtual bool intersects(Shape& shape) = 0;
		virtual bool intersects(Sphere& sphere) = 0;
		virtual bool intersects(AABB& aabb) = 0;
		virtual bool intersects(OBB& obb) = 0;

		virtual ~Shape() = default;

		void setCenterOffset(glm::vec3 centerOffset);
		glm::vec3 centerOffset() const;
		virtual glm::vec3 center();

		virtual float radius() = 0;
		virtual glm::vec3 halfSize() = 0;
		virtual glm::vec3 size() = 0;
		virtual glm::vec3 min() = 0;
		virtual glm::vec3 max() = 0;

		virtual void render() = 0;
	};
}
#endif
