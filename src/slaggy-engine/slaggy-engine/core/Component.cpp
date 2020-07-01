#include "Component.hpp"

#include <core/Entity.hpp>

namespace slaggy
{
	Entity& Component::entity() const
	{
		return *_entity;
	}
	
	Transform* Component::transform() const
	{
		return _entity->getBehavior<Transform>();
	}
}
