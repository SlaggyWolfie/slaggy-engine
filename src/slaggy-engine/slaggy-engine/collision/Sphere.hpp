#pragma once
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <collision/Shape.hpp>

namespace slaggy
{
	class Sphere : public Shape
	{
	public:
		bool intersects(Shape& shape) override;
		bool intersects(Sphere& sphere) override;
		bool intersects(AABB& aabb) override;
		bool intersects(OBB& obb) override;

		void setRadius(float radius);
		float radius() override;
		
		glm::vec3 halfSize() override;
		glm::vec3 size() override;
		glm::vec3 min() override;
		glm::vec3 max() override;

		void render() override;

	private:
		float _radius = 0;
	};
}
#endif