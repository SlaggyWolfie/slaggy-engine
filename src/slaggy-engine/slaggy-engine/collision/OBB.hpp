#pragma once
#ifndef OBB_HPP
#define OBB_HPP

#include <collision/BB.hpp>

namespace slaggy
{
	class OBB : public BB
	{
	public:
		glm::mat4 transform() const;

		bool intersects(Shape& shape) override;
		bool intersects(Sphere& sphere) override;
		bool intersects(AABB& aabb) override;
		bool intersects(OBB& obb) override;
		
		void render() override;
	};
}
#endif