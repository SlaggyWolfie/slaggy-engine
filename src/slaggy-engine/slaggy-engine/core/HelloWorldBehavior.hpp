#pragma once
#ifndef HELLO_WORLD_BEHAVIOR_HPP
#define HELLO_WORLD_BEHAVIOR_HPP

#include <core/Behavior.hpp>

namespace slaggy
{
	class HelloWorldBehavior : public Behavior, public ICloneableTemplate<HelloWorldBehavior>
	{
	public:
		~HelloWorldBehavior() = default;
	
		HelloWorldBehavior* clone() override;
	
		void update() override;
	};
}
#endif