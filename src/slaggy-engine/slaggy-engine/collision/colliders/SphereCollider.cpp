#include "SphereCollider.hpp"

#include <core/Entity.hpp>

namespace slaggy
{
	Component* SphereCollider::clone()
	{
		return nullptr;
	}

	Transform* SphereCollider::transform() const
	{
		return entity().transform();
	}
}
