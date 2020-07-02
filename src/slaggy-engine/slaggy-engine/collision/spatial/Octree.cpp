#include "Octree.hpp"
#include "utils/BoxDebug.hpp"
#include <glm/ext/matrix_transform.inl>

namespace slaggy
{
	glm::vec3 Octree::cell(const unsigned index)
	{
		// example: i = 3 (0b011) -> cell = {1, 1, 0}

		//const glm::vec3 cell(index & 1, index & 2, index & 4);

		const glm::vec3 cell(index & 1, (index & 2) / 2, (index & 4) / 4);
		return cell * 2.0f - glm::vec3(1);
		return (cell * 2.0f - glm::vec3(1)) * glm::vec3(0.5f);
		return cell - glm::vec3(0.5f);
	};

	void Octree::build(const glm::vec3 center, const glm::vec3 halfSize, const unsigned currentDepth, const unsigned maxDepth, const std::vector<Shape*>& objects)
	{
		_currentDepth = currentDepth;
		
		_transform = std::make_unique<Transform>();
		transform()->setPosition(center);
		
		setHalfSize(halfSize);

		if (objects.empty()) return;
		
		std::vector<Shape*> intersecting;
		auto pred = [&](Shape* shape) { return shape->intersects(*this); };

		std::copy_if(objects.begin(), objects.end(), std::back_inserter(intersecting), pred);

		if (intersecting.empty()) return;
		
		//if (intersecting.size() == 1)
		//{
		//	_objects.insert(*intersecting.begin());
		//	return;
		//}

		if (currentDepth == maxDepth)
		{
			_objects.insert(intersecting.begin(), intersecting.end());
			return;
		}

		for (unsigned i = 0; i < 8; ++i)
		{
			_nodes[i] = std::make_unique<Octree>();
			const glm::vec3 childHalfSize = Octree::halfSize() / 2.0f;
			const glm::vec3 cellLocation = cell(i);
			_nodes[i]->build(center + cellLocation * childHalfSize, childHalfSize, currentDepth + 1, maxDepth, intersecting);
		}
	}

	void Octree::reset()
	{
		for (auto&& node : _nodes) node.release();
		_objects.clear();
	}

	std::vector<CollisionPair> Octree::collisions()
	{
		const bool isLeaf = _nodes[0] == nullptr;

		std::vector<CollisionPair> pairs;
		if (isLeaf)
		{
			if (_objects.empty()) return pairs;

			for (auto i_iter = _objects.begin(); i_iter != _objects.end(); ++i_iter)
				for (auto j_iter = std::next(i_iter, 1); j_iter != _objects.end(); ++j_iter)
					pairs.emplace_back(*i_iter, *j_iter);
		}
		else
		{
			for (unsigned i = 0; i < 8; ++i)
			{
				std::vector<CollisionPair> childPairs = _nodes[i]->collisions();
				if (!childPairs.empty())
					std::copy(childPairs.begin(), childPairs.end(), std::back_inserter(pairs));
			}
		}

		return pairs;
	}

	void Octree::render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const
	{
		const glm::mat4 model = glm::scale(scaledTransformationMatrix(), glm::vec3(1 - 0.05f * float(_currentDepth)));
		BoxDebug::instance().render(color, model, view, proj);
	}

	void Octree::renderWithChildren(const glm::mat4& view, const glm::mat4& proj) const
	{
		//const glm::vec3 color =
		//	glm::vec3(1 - 0.1f * float(_currentDepth))
		//	* glm::vec3(_currentDepth & 1, (_currentDepth & 2) / 2, (_currentDepth & 4) / 4);
		
		glm::vec3 color = glm::vec3(1);
		if (_objects.size() == 1) color = glm::vec3(0, 1, 0);
		else if (_objects.size() > 1) color = glm::vec3(1, 0, 0);
		
		render(color, view, proj);

		for (auto&& child : _nodes)
			if (child) child->renderWithChildren(view, proj);
	}

	Transform* Octree::transform() const
	{
		return _transform.get();
	}
}
