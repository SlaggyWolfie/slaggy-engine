#include "Octree.hpp"

namespace slaggy
{
	void Octree::build(const glm::vec3& center, const unsigned currentDepth, const unsigned maxDepth, Octree* parent)
	{
		_position = center;

		if (currentDepth >= maxDepth) return;

		for (unsigned i = 0; i < 8; ++i)
		{
			_nodes[i] = std::make_unique<Octree>();

			// example: i = 3 (0b010) -> cell = {0, 1, 0}
			glm::vec3 cell(i & 1, i & 2, i & 4);
			cell -= glm::vec3(0.5f);

			_nodes[i]->build(center + cell * halfSize() / 2.0f, currentDepth + 1, maxDepth, this);
		}
	}

	Behavior* Octree::clone()
	{
		return nullptr;
	}

	glm::vec3 Octree::center() const
	{
		return _position;
	}

	bool Octree::insert(Shape* shape)
	{
		if (_nodes[0] == nullptr)
		{
			_objects.insert(shape);
			return true;
		}

		// We check if min and max points reside within
		// an octree. If they both do, then they are within this quadrant,
		// if not, then they must be within the parent.
		//
		// This is calculated by comparing both points to the centre mathematically.
		// We then convert the resulting vectors to between a range of [-1; 1] using
		// the sign() function, which range we convert to another range of [0; 1].
		// Due to the nature of sign(), it's more like just -1 and 1, but whatever,
		// which then become 0 and 1, which we use to calculate the octant (3D space
		// "quadrant").
		//
		// (-x, y, -z) -> (-1, 1, -1) -> (0, 1, 0) -> octant 3 (0b010)
		//
		// One caveat for now, is that this only implicitly checks the size of the
		// inserted object.

		// TODO TEST

		const glm::vec3 distMax = shape->max() - _position;
		const glm::vec3 distMin = shape->min() - _position;

		glm::uvec3 distMaxNormalized = glm::uvec3(glm::sign(distMax) / 2.0f + 0.5f);
		glm::uvec3 unn = glm::uvec3(glm::sign(distMin) / 2.0f + 0.5f);

		unsigned octantMax = 0, octantMin = 0;

		// 0..2
		for (unsigned i = 0; i < 3; ++i)
		{
			const auto power = static_cast<unsigned>(std::pow(static_cast<unsigned>(2), i));
			octantMax += distMaxNormalized[i] * power;
			octantMin += unn[i] * power;
		}

		// (true) we found it, boys!
		if (octantMax == octantMin) return _nodes[octantMax]->insert(shape);

		// (false) no one else can handle, it must be me.
		_objects.insert(shape);
		return true;
	}

	//bool StaticOctree::encompasses(const Shape& shape) const
	//{
	//	const glm::vec3 lmin = min();
	//	const glm::vec3 lmax = max();
	//	const glm::vec3 rmin = shape.min();
	//	const glm::vec3 rmax = shape.max();

	//	return lmax.x > rmax.x && rmin.x > lmin.x
	//		&& lmax.y > rmax.y && rmin.y > lmin.y
	//		&& lmax.z > rmax.z && rmin.z > lmin.z;
	//}

	void Octree::clear()
	{
		_objects.clear();
	}

	std::vector<CollisionPair> Octree::collisions()
	{
		const bool isLeaf = _nodes[0] == nullptr;
		const bool isEmpty = _objects.empty();

		if (isLeaf && isEmpty) return std::vector<CollisionPair>();

		std::vector<CollisionPair> pairs;

		if (isLeaf)
		{
			for (unsigned i = 0; i < _objects.size(); ++i)
				for (unsigned j = i + 1; j < _objects.size(); ++j)
					pairs.emplace_back(_objects[i], _objects[j]);

			return pairs;
		}

		for (unsigned i = 0; i < 8; ++i)
		{
			std::vector<CollisionPair> childPairs = _nodes[i]->collisions();
			if (!childPairs.empty())
				std::copy(childPairs.begin(), childPairs.end(), std::back_inserter(pairs));
		}

		if (!isEmpty)
		{
			std::vector<CollisionPair> parentPairs;
			for (const auto& shape : _objects)
				for (const auto& pair : pairs)
					parentPairs.emplace_back(shape, pair.lhs);

			std::copy(parentPairs.begin(), parentPairs.end(), std::back_inserter(pairs));

			for (unsigned i = 0; i < _objects.size(); ++i)
				for (unsigned j = i + 1; j < _objects.size(); ++j)
					pairs.emplace_back(_objects[i], _objects[j]);
		}

		return pairs;
	}
}
