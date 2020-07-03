#include "KDTree.hpp"

namespace slaggy
{
	void KDTree::split(unsigned currentDepth, std::vector<Shape*> objects)
	{
	}

	std::vector<CollisionPair> KDTree::collisions() const
	{
		return std::vector<CollisionPair>();
	}

	void KDTree::renderNodes(const glm::mat4& view, const glm::mat4& proj) const
	{
	}
}
