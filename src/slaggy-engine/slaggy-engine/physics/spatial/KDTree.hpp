#pragma once
#ifndef KD_TREE_HPP
#define KD_TREE_HPP

#include <physics/spatial/SpatialPartitioningTree.hpp>

#include <unordered_set>

namespace slaggy
{
	class KDTree final : public SpatialPartitioningTree, public AABB
	{
	public:
		Transform* transform() const override;

		void initialize(const glm::vec3& center, const glm::vec3& halfSize, unsigned maxDepth) override;
		void reset() override;
		
		std::vector<CollisionPair> collisions() const override;

		void renderNodes(const glm::mat4& view, const glm::mat4& proj) const override;
		void render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const override;
		
	private:
		std::unique_ptr<Transform> _transform{ };
		unsigned _maxDepth = 0;
		unsigned _currentDepth = 0;
		
		std::unique_ptr<KDTree> _lhs = nullptr;
		std::unique_ptr<KDTree> _rhs = nullptr;
		
		std::unordered_set<Shape*> _objects{};

		void split(unsigned depth, Shapes objects) override;
		void construct(const glm::vec3& min, const glm::vec3& max, unsigned depth, unsigned maxDepth, const Shapes& objects);
	};
}
#endif