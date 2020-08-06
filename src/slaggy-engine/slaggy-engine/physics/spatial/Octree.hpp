#pragma once
#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <physics/spatial/SpatialPartitioningTree.hpp>

#include <array>
#include <vector>
#include <memory>
#include <unordered_set>

namespace slaggy
{
	class Octree final : public SpatialPartitioningTree, public AABB
	{
	public:
		Transform* transform() const override;

		void initialize(const glm::vec3& center, const glm::vec3& halfSize, unsigned maxDepth) override;
		void reset() override;
		std::vector<CollisionPair> collisions() const override;

		void render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const override;
		void renderNodes(const glm::mat4& view, const glm::mat4& proj) const override;
		
	private:
		unsigned _maxDepth = 0;
		unsigned _currentDepth = 0;
		
		std::unique_ptr<Transform> _transform = nullptr;

		std::array<std::unique_ptr<Octree>, 8> _nodes{};
		std::unordered_set<Shape*> _objects{};

		static glm::vec3 cell(unsigned index);

		void construct(const glm::vec3& center, const glm::vec3& halfSize, unsigned depth, unsigned maxDepth, const Shapes& objects);
		void split(unsigned depth, Shapes objects) override;
	};
}
#endif