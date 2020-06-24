#ifndef SE_ENTITY_HPP
#define SE_ENTITY_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

namespace slaggy
{
	class Behavior;

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

		template <class TBehavior>
		TBehavior* addBehavior();

		template <class TBehavior>
		bool addBehavior(TBehavior*& behavior);

		template <class TBehavior>
		bool removeBehavior();

		template <class TBehavior>
		TBehavior* getBehavior();

		template <class TBehavior>
		bool hasBehavior();

		template <class TBehavior>
		bool getBehavior(TBehavior*& behavior);

	private:
		std::unordered_map<std::type_index, std::unique_ptr<Behavior>> _behaviors;

		void copy(const Entity& other);

		void addBehavior(Behavior* behavior);

		template <class TBehavior>
		bool isBehavior();
	};

	template <class TBehavior>
	TBehavior* Entity::addBehavior()
	{
		if (!isBehavior<TBehavior>() ||
			_behaviors.count(typeid(TBehavior)) != 0)
			return nullptr;

		TBehavior* behavior = new TBehavior;
		_behaviors.try_emplace(typeid(TBehavior), behavior);
		return behavior;
	}

	template <class TBehavior>
	bool Entity::addBehavior(TBehavior*& behavior)
	{
		behavior = nullptr;
		if (!isBehavior<TBehavior>() ||
			_behaviors.count(typeid(TBehavior)) != 0)
			return false;

		behavior = new TBehavior;
		return _behaviors.try_emplace(typeid(TBehavior), behavior).second;
	}

	template <class TBehavior>
	bool Entity::removeBehavior()
	{
		if (!isBehavior<TBehavior>()) return false;

		const auto iter = _behaviors.find(typeid(TBehavior));
		if (iter == _behaviors.end()) return false;

		_behaviors.erase(iter->first);
		
		return true;
	}

	template <class TBehavior>
	TBehavior* Entity::getBehavior()
	{
		if (!isBehavior<TBehavior>()) return nullptr;

		const auto iter = _behaviors.find(typeid(TBehavior));
		if (iter == _behaviors.end()) return nullptr;

		return static_cast<TBehavior*>(iter->second.get());
	}

	template <class TBehavior>
	bool Entity::getBehavior(TBehavior*& behavior)
	{
		behavior = nullptr;
		if (!isBehavior<TBehavior>()) return false;

		const auto iter = _behaviors.find(typeid(TBehavior));
		if (iter == _behaviors.end()) return false;
		
		behavior = static_cast<TBehavior*>(iter->second.get());
		return true;
	}

	template <class TBehavior>
	bool Entity::hasBehavior()
	{
		return getBehavior<TBehavior>() != nullptr;
	}

	template <class TBehavior>
	bool Entity::isBehavior()
	{
		return std::is_base_of<Behavior, TBehavior>::value;
	}
}
#endif