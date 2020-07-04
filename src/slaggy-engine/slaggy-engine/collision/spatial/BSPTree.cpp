#include "BSPTree.hpp"

#include <algorithm>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <utils/BoxDebug.hpp>
#include <collision/Geometry.hpp>

namespace slaggy
{
	void BSPNode::split(const unsigned depth, Shapes objects)
	{
		if (objects.empty()) return;

		if (depth == _maxDepth || objects.size() == 1)
		{
			_objects.insert(objects.begin(), objects.end());
			return;
		}

		// side comment: oof
		const auto distanceToReference = [&](const glm::vec3& v)
		{
			return Geometry::distanceSqr(v, _referencePoint);
		};

		const auto cmp = [&distanceToReference](Shape* lhs, Shape* rhs)
		{
			return distanceToReference(lhs->center()) < distanceToReference(rhs->center());
		};

		// split
		// sort because choosing a separating plane requires me to do it
		// in lieu of another one, because I can't find any other one
		std::sort(objects.begin(), objects.end(), cmp);

		// choose plane based on middle point between the farthest objects
		const glm::vec3 axis = glm::normalize(objects.back()->center() - objects.front()->center());
		const glm::vec3 point = (objects.back()->center() + objects.front()->center()) / 2.0f;

		_splittingPlane = std::make_unique<PointPlane>();
		_splittingPlane->position = point;
		_splittingPlane->normal = axis;

		Shapes objectsLhs, objectsRhs;

		// if dot is along normal or not & the plane intersects
		const auto front = [&](Shape* o) { return glm::dot(o->center() - point, axis) > 0 || o->intersects(*_splittingPlane); };
		const auto back = [&](Shape* o) { return glm::dot(o->center() - point, axis) < 0 || o->intersects(*_splittingPlane); };
		std::copy_if(objects.begin(), objects.end(), std::back_inserter(objectsLhs), front);
		std::copy_if(objects.begin(), objects.end(), std::back_inserter(objectsRhs), back);

		// create BSP nodes
		_lhs = std::make_unique<BSPNode>();
		_rhs = std::make_unique<BSPNode>();

		_lhs->construct(this, true, depth + 1, _maxDepth, objectsLhs);
		_rhs->construct(this, false, depth + 1, _maxDepth, objectsRhs);
	}

	std::vector<CollisionPair> BSPNode::collisions() const
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

	void BSPNode::construct(BSPNode* const& parent, const bool isLeftNode,
		const unsigned depth, const unsigned maxDepth, const Shapes& objects)
	{
		_parent = parent;
		_isLeftNode = isLeftNode;
		_referencePoint = parent->_referencePoint;

		_currentDepth = depth;
		_maxDepth = maxDepth;

		split(depth, objects);
	}

	void BSPNode::renderNodes(const glm::mat4& view, const glm::mat4& proj) const
	{
		glm::vec3 color = glm::vec3(1);
		if (_objects.size() == 1) color = glm::vec3(0, 1, 0);
		else if (_objects.size() > 1) color = glm::vec3(1, 0, 0);

		if (!_objects.empty())
		{
			glm::mat4 model(1);

			const glm::vec3 worldUp = glm::vec3(0, 1, 0);
			const glm::vec3 rotationAxis = glm::cross(worldUp, _parent->_splittingPlane->normal);
			const float absAngle = glm::orientedAngle(worldUp, _parent->_splittingPlane->normal, rotationAxis);
			const glm::mat4 rotationalMatrix = glm::rotate(absAngle, rotationAxis);

			//const glm::mat4 rotationalMatrix = glm::orientation(_parent->_splittingPlane->normal, worldUp);

			// TODO maybe fix this hard-coded value, but screw planes tbh
			const float diagonal = 17.32f * 1.5f; //10 * sqrt(3) (+extra size for good measure)
			const glm::vec3 size = glm::vec3(diagonal, 0.05f, diagonal) / 2.0f;

			model = glm::translate(model, _parent->_splittingPlane->position + glm::vec3(0, size.y / 2.0f, 0) * (_isLeftNode? 1.0f : -1.0f));
			model = rotationalMatrix * model;
			model = glm::scale(model, size / float(std::pow(2, _currentDepth - 2)));

			BoxDebug::instance().render(color, model, view, proj);
		}

		if (_lhs) _lhs->renderNodes(view, proj);
		if (_rhs) _rhs->renderNodes(view, proj);
	}

	void BSPTree::render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const
	{
		const glm::mat4 model = glm::scale(scaledTransformationMatrix(), glm::vec3(1 - 0.02f * float(_currentDepth)));
		BoxDebug::instance().render(color, model, view, proj);
	}

	void BSPTree::initialize(const glm::vec3& center, const glm::vec3& halfSize, const unsigned maxDepth)
	{
		_transform = std::make_unique<Transform>();

		transform()->setPosition(center);
		setHalfSize(halfSize);
		_maxDepth = maxDepth;

		_referencePoint = min();
	}

	void BSPNode::initialize(const glm::vec3& center, const glm::vec3& halfSize, const unsigned maxDepth)
	{
		// dummy function anyway
		_maxDepth = maxDepth;
	}

	void BSPNode::reset()
	{
		_objects.clear();
		_splittingPlane = nullptr;
		_lhs = nullptr;
		_rhs = nullptr;
	}

	Transform* BSPTree::transform() const
	{
		return _transform.get();
	}

	void BSPTree::renderNodes(const glm::mat4& view, const glm::mat4& proj) const
	{
		if (_objects.empty()) render(glm::vec3(1), view, proj);
		else render(glm::vec3(0, 1, 0), view, proj);
		
		if (_lhs) BSPNode::renderNodes(view, proj);
	}
}
