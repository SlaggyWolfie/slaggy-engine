#pragma once
#ifndef SE_COMPONENT_HPP
#define SE_COMPONENT_HPP

namespace slaggy
{
	class Entity;

	class BaseComponent
	{
		friend class Entity;

	public:
		Entity& entity() const;

	private:
		Entity* _entity = nullptr;;
	};

	inline Entity& BaseComponent::entity() const
	{
		return *_entity;
	}
}
#endif