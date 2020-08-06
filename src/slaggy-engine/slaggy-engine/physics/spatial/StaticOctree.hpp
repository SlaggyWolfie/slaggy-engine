#pragma once
#ifndef STATIC_OCTREE_HPP
#define STATIC_OCTREE_HPP

#include <physics/volumes/AABB.hpp>

#include <array>
#include <vector>
#include <memory>
#include <unordered_set>

namespace slaggy
{
	//struct CollisionPair
	//{
	//	Shape* lhs = nullptr;
	//	Shape* rhs = nullptr;

	//	CollisionPair(Shape* lhs, Shape* rhs) : lhs(lhs), rhs(rhs) { }
	//};
	//
	//class StaticOctree final : public AABB
	//{
	//public:
	//	void build(const glm::vec3& center, unsigned currentDepth, unsigned maxDepth, StaticOctree* parent);

	//	glm::vec3 center() const override;

	//	bool insert(Box* box);
	//	void clear();
	//	//bool remove(Shape* shape);

	//	std::vector<CollisionPair> collisions();

	//protected:
	//	Transform& transform() const override;
	//	
	//private:
	//	glm::vec3 _position = glm::vec3(0);
	//	
	//	std::array<std::unique_ptr<StaticOctree>, 8> _nodes { };
	//	std::unordered_set<Shape*> _objects { };

	//	//bool encompasses(const Shape& shape) const;
	//};
}
#endif