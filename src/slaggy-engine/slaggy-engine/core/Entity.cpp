#include "Entity.hpp"

#include <core/Component.hpp>
#include <core/Transform.hpp>

namespace slaggy
{
	Transform* Entity::transform() const
	{
		return getComponent<Transform>();
	}

	void Entity::copy(const Entity& other)
	{
		for (auto&& pair : other._components)
			addComponent((*pair.second).clone());
	}

	void Entity::addComponent(Component* component)
	{
		component->_entity = this;
		_components.try_emplace(typeid(component), component);
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
	//	_Components.clear();
	//	copy(other);

	//	return *this;
	//}

	//Entity& Entity::operator=(Entity&& other) noexcept
	//{
	//	_Components.clear();
	//	copy(other);

	//	return *this;
	//}
}
