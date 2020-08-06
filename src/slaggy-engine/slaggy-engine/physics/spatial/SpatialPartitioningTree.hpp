#pragma once
#ifndef SPATIAL_PARTITIONING_TREE_HPP
#define SPATIAL_PARTITIONING_TREE_HPP

#include <physics/volumes/AABB.hpp>

#include <vector>
#include <memory>

#include <physics/CollisionPair.hpp>
#include <core/Transform.hpp>

namespace slaggy
{
	class Transform;

	class SpatialPartitioningTree
	{
	public:
		typedef std::vector<Shape*> Shapes;

		virtual ~SpatialPartitioningTree() = default;
		
		virtual void initialize(const glm::vec3& center, const glm::vec3& halfSize, unsigned maxDepth) = 0;

		void startSplit(Shapes objects);
		virtual void reset() = 0;

		virtual std::vector<CollisionPair> collisions() const = 0;
		virtual void renderNodes(const glm::mat4& view, const glm::mat4& proj) const = 0;

	protected:
		virtual void split(unsigned depth, Shapes objects) = 0;
	};
}

#endif