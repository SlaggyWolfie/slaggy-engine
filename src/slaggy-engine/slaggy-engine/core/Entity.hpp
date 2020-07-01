#ifndef SE_ENTITY_HPP
#define SE_ENTITY_HPP

#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include <core/Component.hpp>
//#include <core/Component.hpp>
#include <core/Transform.hpp>

namespace slaggy
{
	class Entity final
	{
	public:
		Entity() = default;
		~Entity() = default;

		Entity(const Entity& other);
		Entity(Entity&& other) noexcept;

		//Entity& operator=(const Entity& other);
		//Entity& operator=(Entity&& other) noexcept;

		Entity& operator=(const Entity& other) = delete;
		Entity& operator=(Entity&& other) noexcept = delete;

		template <class TComponent>
		TComponent* addComponent();

		template <class TComponent>
		bool addComponent(TComponent*& component);

		template <class TComponent>
		bool removeComponent();

		template <class TComponent>
		TComponent* getComponent() const;

		template <class TComponent>
		bool hasComponent() const;

		template <class TComponent>
		bool getComponent(TComponent*& component) const;

		Transform* transform() const;

	private:
		std::unordered_map<std::type_index, std::unique_ptr<Component>> _components;

		void copy(const Entity& other);

		void addComponent(Component* component);

		template <class TComponent>
		static bool isComponent();
	};

	template <class TComponent>
	TComponent* Entity::addComponent()
	{
		if (!isComponent<TComponent>() ||
			_components.count(typeid(TComponent)) != 0)
			return nullptr;

		TComponent* component = new TComponent;
		static_cast<Component*>(component)->_entity = this;
		
		_components.try_emplace(typeid(TComponent), component);
		return component;
	}

	template <class TComponent>
	bool Entity::addComponent(TComponent*& component)
	{
		component = nullptr;
		if (!isComponent<TComponent>() ||
			_components.count(typeid(TComponent)) != 0)
			return false;

		component = new TComponent;
		static_cast<Component*>(component)->_entity = this;
		
		return _components.try_emplace(typeid(TComponent), component).second;
	}

	template <class TComponent>
	bool Entity::removeComponent()
	{
		if (!isComponent<TComponent>()) return false;

		const auto iter = _components.find(typeid(TComponent));
		if (iter == _components.end()) return false;

		_components.erase(iter->first);
		
		return true;
	}

	template <class TComponent>
	TComponent* Entity::getComponent() const
	{
		if (!isComponent<TComponent>()) return nullptr;

		const auto iter = _components.find(typeid(TComponent));
		if (iter == _components.end()) return nullptr;

		return dynamic_cast<TComponent*>(iter->second.get());
	}

	template<>
	inline Transform* Entity::getComponent() const
	{
		const auto iter = _components.find(typeid(Transform));
		if (iter == _components.end()) return nullptr;

		return dynamic_cast<Transform*>(iter->second.get());
	}

	template <class TComponent>
	bool Entity::getComponent(TComponent*& component) const
	{
		component = nullptr;
		if (!isComponent<TComponent>()) return false;

		const auto iter = _components.find(typeid(TComponent));
		if (iter == _components.end()) return false;
		
		component = dynamic_cast<TComponent*>(iter->second.get());
		return true;
	}

	template <class TComponent>
	bool Entity::hasComponent() const
	{
		return getComponent<TComponent>() != nullptr;
	}

	template <class TComponent>
	bool Entity::isComponent()
	{
		return std::is_base_of<Component, TComponent>::value;
	}
}
#endif