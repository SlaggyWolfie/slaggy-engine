#include "SpatialPartitioningTree.hpp"

namespace slaggy
{
	Transform* SpatialPartitioningTree::transform() const
	{
		return _transform.get();
	}
	
	void SpatialPartitioningTree::initialize(const glm::vec3& center, const glm::vec3& halfSize, const unsigned maxDepth)
	{
		_transform = std::make_unique<Transform>();
		
		transform()->setPosition(center);
		setHalfSize(halfSize);
		_maxDepth = maxDepth;
	}

	void SpatialPartitioningTree::split(std::vector<Shape*> objects)
	{
		split(0, std::move(objects));
	}
}
