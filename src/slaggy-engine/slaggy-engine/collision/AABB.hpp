#pragma once

#ifndef AABB_HPP
#define AABB_HPP

#include <collision/BB.hpp>

namespace slaggy
{
	class AABB : public BB
	{
	public:
		bool intersects(Shape& shape) override;
		bool intersects(Sphere& sphere) override;
		bool intersects(AABB& aabb) override;
		bool intersects(OBB& obb) override;

		void render() override;
	};
}
#endif //AABB_HPP