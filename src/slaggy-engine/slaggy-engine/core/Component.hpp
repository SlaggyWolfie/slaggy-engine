#pragma once
#ifndef SE_COMPONENT_HPP
#define SE_COMPONENT_HPP
#include "ICloneable.hpp"

namespace slaggy
{
	class Entity;
	class Transform;

	class Component : public ICloneableTemplate<Component>
	{
		friend class Entity;

	public:
		Entity& entity() const;
		virtual Transform* transform() const;

	private:
		Entity* _entity = nullptr;;
	};
}
#endif