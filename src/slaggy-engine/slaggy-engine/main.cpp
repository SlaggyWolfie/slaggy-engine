#include "Entity.hpp"
#include "HelloWorldBehavior.hpp"
#include <iostream>
#include <string>

using namespace slaggy;

int main()
{
	Entity entity;

	std::cout << std::boolalpha;
	std::cout << "Add component to Entity" << std::endl;
	auto a = entity.addBehavior<HelloWorldBehavior>();
	std::cout << "Entity has component: " << entity.hasBehavior<HelloWorldBehavior>() << std::endl;

	std::cout << "Removed component from Entity" << std::endl;
	std::cout << "Entity has no component: " << entity.removeBehavior<HelloWorldBehavior>() << std::endl;

	std::cout << "Added component & component says: ";
	HelloWorldBehavior* b;
	if (entity.addBehavior(b)) b->update();

	std::cout << "Test Has: " << entity.hasBehavior<HelloWorldBehavior>() << std::endl;
	std::cout << "Got component & component says: ";
	if (entity.getBehavior(b)) b->update();

	return 0;
}