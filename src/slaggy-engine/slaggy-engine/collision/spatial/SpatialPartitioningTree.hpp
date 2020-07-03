#pragma once
#ifndef SPATIAL_PARTITIONING_TREE_HPP
#define SPATIAL_PARTITIONING_TREE_HPP

#include <collision/volumes/AABB.hpp>

#include <vector>
#include <memory>

#include <collision/CollisionPair.hpp>
#include <core/Transform.hpp>

namespace slaggy
{
	class Transform;

	class SpatialPartitioningTree : public AABB
	{
	protected:
		typedef std::vector<Shape*> Shapes;
	public:
		Transform* transform() const override;

		void initialize(const glm::vec3& center, const glm::vec3& halfSize, unsigned maxDepth);

		void startSplit(Shapes objects);
		virtual void reset() = 0;

		virtual std::vector<CollisionPair> collisions() const = 0;
		virtual void renderNodes(const glm::mat4& view, const glm::mat4& proj) const = 0;

	protected:		
		unsigned _maxDepth = 0;
		unsigned _currentDepth = 0;
		std::unique_ptr<Transform> _transform{ };

		virtual void split(unsigned depth, Shapes objects) = 0;
	};
}

#endif