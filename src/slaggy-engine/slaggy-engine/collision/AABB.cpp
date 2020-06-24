#include "AABB.hpp"

#include <collision/Collisions.hpp>

//#include "Octree.hpp"
//#include "mge/core/World.hpp"
//#include "mge/core/Camera.hpp"

namespace slaggy
{
	bool AABB::intersects(Shape& shape)
	{
		//std::cout << "Intersection between AABB and Shape" << std::endl;
		return shape.intersects(*this);
	}

	bool AABB::intersects(Sphere& sphere)
	{
		//std::cout << "Intersection between AABB and Sphere" << std::endl;
		return Collisions::intersection(sphere, *this);
	}

	bool AABB::intersects(AABB& aabb)
	{
		//std::cout << "Intersection between AABB and AABB" << std::endl;
		return Collisions::intersection(*this, aabb);
	}

	bool AABB::intersects(OBB& obb)
	{
		//std::cout << "Intersection between AABB and OBB" << std::endl;
		return Collisions::intersection(*this, obb);
	}
	
	void AABB::render()
	{
		// TODO
		//glm::mat4 model = glm::mat4(1);
		//model = glm::translate(model, center());
		//model = glm::scale(model, _halfSize);

		//Octree::getVisualBox()->render(
		//	glm::vec3(0, 1, 0), 
		//	model, 
		//	world->getMainCamera()->getView(),
		//	world->getMainCamera()->getProjection());
	}
}
