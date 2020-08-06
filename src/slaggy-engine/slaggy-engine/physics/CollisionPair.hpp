#pragma once
#ifndef COLLISION_PAIR_HPP
#define COLLISION_PAIR_HPP

namespace slaggy
{
	class Shape;
	
	struct CollisionPair
	{
		Shape* lhs = nullptr;
		Shape* rhs = nullptr;

		CollisionPair(Shape* lhs, Shape* rhs) : lhs(lhs), rhs(rhs) { }
	};
}
#endif