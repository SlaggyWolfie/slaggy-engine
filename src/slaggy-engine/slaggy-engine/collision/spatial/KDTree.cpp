#include "KDTree.hpp"

#include <algorithm>

#include <glm/ext/matrix_transform.hpp>

#include <utils/BoxDebug.hpp>

namespace slaggy
{
	void KDTree::split(const unsigned depth, Shapes objects)
	{
		if (objects.empty()) return;

		Shapes intersecting;
		auto pred = [&](Shape* shape) { return shape->intersects(*this); };
		std::copy_if(objects.begin(), objects.end(), std::back_inserter(intersecting), pred);

		if (intersecting.empty()) return;
		if (depth == _maxDepth || intersecting.size() == 1)
		{
			_objects.insert(intersecting.begin(), intersecting.end());
			return;
		}

		// split
		const unsigned axisIndex = depth % 3;
		float point = 0;

		// get median, in this case average, because
		// it doesn't require additional sorting
		for (auto& shape : intersecting)
			point += shape->center()[axisIndex];
		point /= intersecting.size();

		// after we find the splitting point along the axis
		// create box defining points, in this case min and max
		const glm::vec3 min = this->min();
		const glm::vec3 max = this->max();

		const glm::vec3 minLhs = min;
		const glm::vec3 maxRhs = max;

		glm::vec3 maxLhs = max;
		glm::vec3 minRhs = min;

		maxLhs[axisIndex] = point;
		minRhs[axisIndex] = point;

		// initialize & split KD trees
		_lhs = std::make_unique<KDTree>();
		_rhs = std::make_unique<KDTree>();

		_lhs->construct(minLhs, maxLhs, depth + 1, _maxDepth, intersecting);
		_rhs->construct(minRhs, maxRhs, depth + 1, _maxDepth, intersecting);
	}

	void KDTree::reset()
	{
		_objects.clear();
		_lhs = nullptr;
		_rhs = nullptr;
	}

	std::vector<CollisionPair> KDTree::collisions() const
	{
		std::vector<CollisionPair> pairs;
		if (!_lhs)
		{
			if (_objects.empty()) return pairs;

			for (auto i_iter = _objects.begin(); i_iter != _objects.end(); ++i_iter)
				for (auto j_iter = std::next(i_iter, 1); j_iter != _objects.end(); ++j_iter)
					pairs.emplace_back(*i_iter, *j_iter);
		}
		else
		{
			std::vector<CollisionPair> childPairs = _lhs->collisions();
			if (!childPairs.empty())
				std::copy(childPairs.begin(), childPairs.end(), std::back_inserter(pairs));

			childPairs = _rhs->collisions();
			if (!childPairs.empty())
				std::copy(childPairs.begin(), childPairs.end(), std::back_inserter(pairs));
		}

		return pairs;
	}

	void KDTree::renderNodes(const glm::mat4& view, const glm::mat4& proj) const
	{
		glm::vec3 color = glm::vec3(1);
		if (_objects.size() == 1) color = glm::vec3(0, 1, 0);
		else if (_objects.size() > 1) color = glm::vec3(1, 0, 0);

		if (!_objects.empty()) render(color, view, proj);

		if (_lhs) _lhs->renderNodes(view, proj);
		if (_rhs) _rhs->renderNodes(view, proj);
	}

	void KDTree::render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const
	{
		const glm::mat4 model = glm::scale(scaledTransformationMatrix(), glm::vec3(1 - 0.02f * float(_currentDepth)));
		BoxDebug::instance().render(color, model, view, proj);
	}

	void KDTree::construct(const glm::vec3& min, const glm::vec3& max,
	                       const unsigned depth, const unsigned maxDepth, const Shapes& objects)
	{
		_currentDepth = depth;
		initialize((min + max) / 2.0f, (max - min) / 2.0f, maxDepth);
		split(depth, objects);
	}
}
