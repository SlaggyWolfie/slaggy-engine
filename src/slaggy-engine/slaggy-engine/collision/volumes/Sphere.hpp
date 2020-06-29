#pragma once
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <collision/volumes/Shape.hpp>

namespace slaggy
{
	class Sphere final : public Shape
	{
	public:
		bool intersects(Shape& shape) override;
		bool intersects(Sphere& sphere) override;
		bool intersects(AABB& aabb) override;
		bool intersects(OBB& obb) override;

		void setRadius(float radius);
		float radius() const override;

		bool isInside(glm::vec3 point) const override;
		glm::vec3 closestPointTo(glm::vec3 point) const override;
		
		void render() override;
		
	private:
		float _radius = 0;
	};
}
#endif