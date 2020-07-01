#pragma once
#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <array>
#include <vector>
#include <memory>
#include <unordered_set>

#include <core/Transform.hpp>
#include <collision/volumes/AABB.hpp>

namespace slaggy
{
	struct CollisionPair
	{
		Shape* lhs = nullptr;
		Shape* rhs = nullptr;

		CollisionPair(Shape* lhs, Shape* rhs) : lhs(lhs), rhs(rhs) { }
	};

	class Octree final : public AABB
	{
	public:
		Transform& transform() const override;

		void build(glm::vec3 center, glm::vec3 halfSize, unsigned currentDepth, unsigned maxDepth, const std::vector<Shape*>& objects);
		void reset();
		std::vector<CollisionPair> collisions();

		void render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const override;
		void renderWithChildren(const glm::mat4& view, const glm::mat4& proj) const;
		
		//std::array<Octree*, 8> nodes()
		//{
		//	std::array<Octree*, 8> o { };
		//	for (int i = 0; i < 8; i++)
		//		o[i] = _nodes[i].get();
		//	
		//	return o;
		//}
		
	private:
		unsigned _currentDepth = 0;
		
		std::unique_ptr<Transform> _transform{ };
		std::array<std::unique_ptr<Octree>, 8> _nodes{ };
		std::unordered_set<Shape*> _objects{ };

		static glm::vec3 cell(unsigned index);
	};
}
#endif