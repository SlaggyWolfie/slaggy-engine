#pragma once
#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <collision/volumes/AABB.hpp>

#include <array>
#include <vector>
#include <memory>
#include <unordered_set>

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
		void build(const glm::vec3& center, unsigned currentDepth, unsigned maxDepth, Octree* parent);

		Behavior* clone() override;
		glm::vec3 center() const override;

		bool insert(Shape* shape);
		void clear();
		//bool remove(Shape* shape);

		std::vector<CollisionPair> collisions();
		
	private:
		glm::vec3 _position = glm::vec3(0);
		
		std::array<std::unique_ptr<Octree>, 8> _nodes { };
		std::unordered_set<Shape*> _objects { };

		//bool encompasses(const Shape& shape) const;
	};
}
#endif