#include "StaticOctree.hpp"

namespace slaggy
{
	//void StaticOctree::build(const glm::vec3& center, const unsigned currentDepth, const unsigned maxDepth, StaticOctree* parent)
	//{
	//	_position = center;

	//	if (currentDepth >= maxDepth) return;

	//	for (unsigned i = 0; i < 8; ++i)
	//	{
	//		_nodes[i] = std::make_unique<StaticOctree>();

	//		// example: i = 3 (0b010) -> cell = {0, 1, 0}
	//		glm::vec3 cell(i & 1, i & 2, i & 4);
	//		cell -= glm::vec3(0.5f);

	//		_nodes[i]->build(center + cell * halfSize() / 2.0f, currentDepth + 1, maxDepth, this);
	//	}
	//}

	//glm::vec3 StaticOctree::center() const
	//{
	//	return _position;
	//}

	//bool StaticOctree::insert(Box* box)
	//{
	//	// TODO
	//	return false;
	//	 
	//	if (_nodes[0] == nullptr)
	//	{
	//		_objects.insert(box);
	//		return true;
	//	}

	//	// We check if min and max points reside within
	//	// an octree. If they both do, then they are within this quadrant,
	//	// if not, then they must be within the parent.
	//	//
	//	// This is calculated by comparing both points to the centre mathematically.
	//	// We then convert the resulting vectors to between a range of [-1; 1] using
	//	// the sign() function, which range we convert to another range of [0; 1].
	//	// Due to the nature of sign(), it's more like just -1 and 1, but whatever,
	//	// which then become 0 and 1, which we use to calculate the octant (3D space
	//	// "quadrant").
	//	//
	//	// (-x, y, -z) -> (-1, 1, -1) -> (0, 1, 0) -> octant 3 (0b010)
	//	//
	//	// One caveat for now, is that this only implicitly checks the size of the
	//	// inserted object.

	//	// TODO TEST

	//	const glm::vec3 distMax = box->max() - _position;
	//	const glm::vec3 distMin = box->min() - _position;

	//	glm::uvec3 distMaxNormalized = glm::uvec3(glm::sign(distMax) / 2.0f + 0.5f);
	//	glm::uvec3 unn = glm::uvec3(glm::sign(distMin) / 2.0f + 0.5f);

	//	unsigned octantMax = 0, octantMin = 0;

	//	// 0..2
	//	for (unsigned i = 0; i < 3; ++i)
	//	{
	//		const auto power = static_cast<unsigned>(std::pow(static_cast<unsigned>(2), i));
	//		octantMax += distMaxNormalized[i] * power;
	//		octantMin += unn[i] * power;
	//	}

	//	// (true) we found it, boys!
	//	if (octantMax == octantMin) return _nodes[octantMax]->insert(box);

	//	// (false) no one else can handle, it must be me.
	//	_objects.insert(box);
	//	return true;
	//}

	////bool StaticOctree::encompasses(const Shape& shape) const
	////{
	////	const glm::vec3 lmin = min();
	////	const glm::vec3 lmax = max();
	////	const glm::vec3 rmin = shape.min();
	////	const glm::vec3 rmax = shape.max();

	////	return lmax.x > rmax.x && rmin.x > lmin.x
	////		&& lmax.y > rmax.y && rmin.y > lmin.y
	////		&& lmax.z > rmax.z && rmin.z > lmin.z;
	////}

	//void StaticOctree::clear()
	//{
	//	_objects.clear();
	//}

	//std::vector<CollisionPair> StaticOctree::collisions()
	//{
	//	const bool isLeaf = _nodes[0] == nullptr;

	//	std::vector<CollisionPair> pairs;
	//	if (isLeaf)
	//	{
	//		if (_objects.empty()) return pairs;

	//		for (auto i_iter = _objects.begin(); i_iter != _objects.end(); ++i_iter)
	//			for (auto j_iter = std::next(i_iter, 1); j_iter != _objects.end(); ++j_iter)
	//				pairs.emplace_back(*i_iter, *j_iter);
	//	}
	//	else 
	//	{
	//		for (unsigned i = 0; i < 8; ++i)
	//		{
	//			std::vector<CollisionPair> childPairs = _nodes[i]->collisions();
	//			if (!childPairs.empty())
	//				std::copy(childPairs.begin(), childPairs.end(), std::back_inserter(pairs));
	//		}
	//	}

	//	return pairs;
	//}
}
