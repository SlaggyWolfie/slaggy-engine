#include "CollisionManager.hpp"

#include <physics/Geometry.hpp>
#include <OctreeMovement.hpp>
#include <core/Component.hpp>
#include <core/Entity.hpp>
#include <physics/colliders/SphereCollider.hpp>

namespace slaggy
{
	//std::unique_ptr<CollisionManager> CollisionManager::_instance = nullptr;
	//
	//CollisionManager& CollisionManager::instance()
	//{
	//	if (!_instance) _instance = std::make_unique<CollisionManager>();
	//	return *_instance;
	//}

	void CollisionManager::resolve(const std::vector<CollisionPair>& pairs)
	{
		for (auto pair : pairs) resolve(pair);
	}

	void CollisionManager::resolve(const CollisionPair& pair)
	{
		if (!pair.lhs->intersects(*pair.rhs)) return;
		
		// TODO Fix Hard code
		auto lhs = dynamic_cast<SphereCollider*>(pair.lhs)->entity().getComponent<OctreeMovement>();
		auto rhs = dynamic_cast<SphereCollider*>(pair.rhs)->entity().getComponent<OctreeMovement>();
		Geometry::reflectVelocity(pair.lhs->center(), pair.rhs->center(),lhs->velocity, rhs->velocity);
	}
}
