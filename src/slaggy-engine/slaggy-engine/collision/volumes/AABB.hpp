#pragma once

#ifndef AABB_HPP
#define AABB_HPP

#include <collision/volumes/Box.hpp>

namespace slaggy
{
	class AABB : public Box
	{
	public:
		bool intersects(Shape& shape) override;
		bool intersects(Sphere& sphere) override;
		bool intersects(AABB& aabb) override;
		bool intersects(OBB& obb) override;

		void render() override;
		
		Behavior* clone() override;
		bool isInside(glm::vec3 point) const override;
		glm::vec3 closestPointTo(glm::vec3 point) const override;
	};
}
#endif //AABB_HPP