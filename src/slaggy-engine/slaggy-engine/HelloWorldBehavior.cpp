#include "HelloWorldBehavior.hpp"

#include <iostream>

namespace slaggy
{
	HelloWorldBehavior* HelloWorldBehavior::clone()
	{
		return new HelloWorldBehavior();
	}

	void HelloWorldBehavior::update()
	{
		std::cout << "Hello World!" << std::endl;
	}
}