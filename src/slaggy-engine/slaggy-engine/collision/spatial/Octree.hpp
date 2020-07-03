#pragma once
#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <collision/spatial/SpatialPartitioningTree.hpp>

#include <array>
#include <vector>
#include <memory>
#include <unordered_set>

namespace slaggy
{
	class Octree final : public SpatialPartitioningTree
	{
	public:
		using SpatialPartitioningTree::split;

		void reset();
		void construct(const glm::vec3& center, const glm::vec3& halfSize, unsigned depth, unsigned maxDepth, const std::vector<Shape*>& objects);
		std::vector<CollisionPair> collisions() const override;

		void render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const override;
		void renderNodes(const glm::mat4& view, const glm::mat4& proj) const override;

		void split(unsigned depth, std::vector<Shape*> objects) override;
		
	private:
		unsigned _currentDepth = 0;

		std::array<std::unique_ptr<Octree>, 8> _nodes{};
		std::unordered_set<Shape*> _objects{};

		static glm::vec3 cell(unsigned index);
	};
}
#endif