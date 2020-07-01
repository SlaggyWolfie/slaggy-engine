#pragma once
#ifndef SE_COMPONENT_HPP
#define SE_COMPONENT_HPP

namespace slaggy
{
	class Entity;
	class Transform;

	class Component
	{
		friend class Entity;

	public:
		Entity& entity() const;
		Transform* transform() const;

	private:
		Entity* _entity = nullptr;;
	};
}
#endif