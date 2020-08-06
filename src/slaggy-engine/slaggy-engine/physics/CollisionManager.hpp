#pragma once
#ifndef COLLISION_MANAGER_HPP
#define COLLISION_MANAGER_HPP
#include <memory>
#include <vector>
#include <physics/spatial/Octree.hpp>

namespace slaggy
{
	class CollisionManager
	{
	public:
		//static CollisionManager& instance();

		static void resolve(const std::vector<CollisionPair>& pairs);
		static void resolve(const CollisionPair& pair);

	private:
		//static std::unique_ptr<CollisionManager> _instance;
	};
}
#endif