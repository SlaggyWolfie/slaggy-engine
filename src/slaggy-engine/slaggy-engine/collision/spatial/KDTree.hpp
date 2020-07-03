#pragma once
#ifndef KD_TREE_HPP
#define KD_TREE_HPP

#include <collision/spatial/SpatialPartitioningTree.hpp>

namespace slaggy
{
	class KDTree final : public SpatialPartitioningTree
	{
	public:
		void split(unsigned currentDepth, std::vector<Shape*> objects) override;
		std::vector<CollisionPair> collisions() const override;
		void renderNodes(const glm::mat4& view, const glm::mat4& proj) const override;
		
	private:
		std::unique_ptr<KDTree> _lhs = nullptr;
		std::unique_ptr<KDTree> _rhs = nullptr;
	};
}
#endif