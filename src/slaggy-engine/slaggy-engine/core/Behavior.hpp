#pragma once
#ifndef SE_BEHAVIOUR_HPP
#define SE_BEHAVIOUR_HPP

#include <core/Component.hpp>
#include <core/ICloneable.hpp>

namespace slaggy
{
	/// <summary>
	/// Behaviors are meant to be extended and used as components for an Entity give them behavior
	/// or rather the ability to do something. They are allowed to hold data in addition to behavior.
	/// </summary>
	class Behavior : public Component, public ICloneableTemplate<Behavior>
	{
	public:
		Behavior() = default;
		virtual ~Behavior() = default;

		Behavior(const Behavior& other) = delete;
		Behavior(Behavior&& other) noexcept = delete;

		Behavior& operator=(const Behavior& other) = delete;
		Behavior& operator=(Behavior&& other) noexcept = delete;

		virtual void update() { };
		virtual void fixedUpdate() { };
		virtual void lateUpdate() { };
	};
}
#endif