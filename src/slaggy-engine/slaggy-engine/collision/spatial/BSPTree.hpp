#pragma once
#ifndef BSP_TREE_HPP
#define BSP_TREE_HPP

#include <collision/spatial/SpatialPartitioningTree.hpp>

#include <unordered_set>

#include <collision/PointPlane.hpp>

namespace slaggy
{
	class BSPNode : public SpatialPartitioningTree
	{
	public:
		void initialize(const glm::vec3& center, const glm::vec3& halfSize, unsigned maxDepth) override;
		void reset() override;

		std::vector<CollisionPair> collisions() const override;
		void renderNodes(const glm::mat4& view, const glm::mat4& proj) const override;

	protected:
		glm::vec3 _referencePoint = glm::vec3(0);

		unsigned _maxDepth = 0;
		unsigned _currentDepth = 0;

		BSPNode* _parent = nullptr;
		bool _isLeftNode = false;
		
		std::unique_ptr<PointPlane> _splittingPlane = nullptr;

		std::unique_ptr<BSPNode> _lhs = nullptr;
		std::unique_ptr<BSPNode> _rhs = nullptr;

		std::unordered_set<Shape*> _objects{};

		void split(unsigned depth, Shapes objects) override;
		void construct(BSPNode* const& parent, bool isLeftNode, unsigned depth, unsigned maxDepth, const Shapes& objects);
	};

	class BSPTree final : public BSPNode, public AABB
	{
	public:
		Transform* transform() const override;

		void initialize(const glm::vec3& center, const glm::vec3& halfSize, unsigned maxDepth) override;
		void render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const override;

		void renderNodes(const glm::mat4& view, const glm::mat4& proj) const override;

	private:
		std::unique_ptr<Transform> _transform = nullptr;
	};
}
#endif