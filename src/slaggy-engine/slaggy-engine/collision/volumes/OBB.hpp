#pragma once
#ifndef OBB_HPP
#define OBB_HPP

#include <collision/volumes/Box.hpp>

namespace slaggy
{
	class OBB : public Box
	{
	public:
		bool intersects(Shape& shape) override;
		bool intersects(Sphere& sphere) override;
		bool intersects(AABB& aabb) override;
		bool intersects(OBB& obb) override;

		glm::vec3 min() const override;
		glm::vec3 max() const override;
		glm::vec3 unorientedMin() const;
		glm::vec3 unorientedMax() const;
		
		void render() override;
		Behavior* clone() override;
		bool isInside(glm::vec3 point) const override;
		glm::vec3 closestPointTo(glm::vec3 point) const override;
	};
}
#endif