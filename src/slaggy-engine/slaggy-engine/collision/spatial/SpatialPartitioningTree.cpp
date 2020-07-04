#include "SpatialPartitioningTree.hpp"

namespace slaggy
{
	void SpatialPartitioningTree::startSplit(Shapes objects)
	{
		reset();
		split(0, std::move(objects));
	}
}
