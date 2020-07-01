#pragma once
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <collision/volumes/Shape.hpp>

namespace slaggy
{
	class Sphere final : public Shape
	{
	public:
		bool intersects(const Shape& shape) const override;
		bool intersects(const Sphere& sphere) const override;
		bool intersects(const AABB& aabb) const override;
		bool intersects(const OBB& obb) const override;

		void setRadius(float radius);
		float radius() const override;

		bool isInside(glm::vec3 point) const override;
		glm::vec3 closestPointTo(glm::vec3 point) const override;
		
		void render(const glm::mat4& view, const glm::mat4& proj) const override;
		
	private:
		float _radius = 0;
	};
}
#endif