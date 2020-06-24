#pragma once
#ifndef HELLO_WORLD_BEHAVIOR_HPP
#define HELLO_WORLD_BEHAVIOR_HPP
#include "Behavior.hpp"

namespace slaggy
{
	class HelloWorldBehavior : public slaggy::Behavior, ICloneableTemplate<HelloWorldBehavior>
	{
	public:
		~HelloWorldBehavior() = default;
	
		HelloWorldBehavior* clone() override;
	
		void update() override;
		//void fixedUpdate() override;
		//void lateUpdate() override;
	};
}
#endif