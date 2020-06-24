#include "Entity.hpp"
#include "Behavior.hpp"

namespace slaggy
{
	void Entity::copy(const Entity& other)
	{
		for (auto&& pair : other._behaviors)
			addBehavior((*pair.second).clone());
	}

	void Entity::addBehavior(Behavior* behavior)
	{
		_behaviors.try_emplace(typeid(behavior), behavior);
	}
	
	Entity::Entity(const Entity& other)
	{
		copy(other);
	}

	Entity::Entity(Entity&& other) noexcept
	{
		copy(other);
	}

	//Entity& Entity::operator=(const Entity& other)
	//{
	//	if (this == &other) return *this;
	//	
	//	_behaviors.clear();
	//	copy(other);

	//	return *this;
	//}

	//Entity& Entity::operator=(Entity&& other) noexcept
	//{
	//	_behaviors.clear();
	//	copy(other);

	//	return *this;
	//}
}
