#include "Entity.hpp"
#include "HelloWorldBehavior.hpp"
#include <iostream>
#include "Transform.hpp"
#include <glm/gtx/string_cast.hpp>
#include "PhysicsBehavior.hpp"
#include "SimplePhysicsBehavior.hpp"

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

	Transform* transform;
	if (entity.addBehavior(transform)) 
	{
		transform->translate(glm::vec3(25));
		std::cout << "Position: " << glm::to_string(transform->position()) << std::endl;
	}

	SimplePhysicsBehavior* pb;
	if (entity.addBehavior(pb))
	{
		pb->setConstantVelocity(glm::vec3(37));
		pb->fixedUpdate();
		pb->fixedUpdate();
		std::cout << "Physics Position: " << glm::to_string(transform->position()) << std::endl;
	}

	return 0;
}