#include "OBB.hpp"

#include <core/Entity.hpp>
#include <core/Transform.hpp>

#include "Collisions.hpp"

//#include "Octree.hpp"
//#include "mge/core/World.hpp"
//#include "mge/core/Camera.hpp"

namespace slaggy
{
	glm::mat4 OBB::transform() const
	{
		return entity().getBehavior<Transform>()->transformationMatrix();
	}

	bool OBB::intersects(Shape& shape)
	{
		//std::cout << "Intersection between OBB and Shape" << std::endl;
		return shape.intersects(*this);
	}

	bool OBB::intersects(Sphere& sphere)
	{
		//std::cout << "Intersection between OBB and Sphere" << std::endl;
		return Collisions::intersection(sphere, *this);
	}

	bool OBB::intersects(AABB& aabb)
	{
		//std::cout << "Intersection between OBB and AABB" << std::endl;
		return Collisions::intersection(aabb, *this);
	}

	bool OBB::intersects(OBB& obb)
	{
		//std::cout << "Intersection between OBB and OBB" << std::endl;
		return Collisions::intersection(obb, *this);
	}

	void OBB::render()
	{
		//glm::mat4 model = getWorldTransform();
		//model = glm::translate(model, getCenterOffset());
		//model = glm::scale(model, _halfSize);

		//Octree::getVisualBox()->render(
		//	glm::vec3(1, 0, 0),
		//	model,
		//	world->getMainCamera()->getView(),
		//	world->getMainCamera()->getProjection());
	}
	
}
