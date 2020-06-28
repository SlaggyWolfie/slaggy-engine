#include "Box.hpp"
#include <algorithm>

namespace slaggy
{
	void Box::setHalfSize(const glm::vec3 halfSize)
	{
		_halfSize = halfSize;
		_radius = std::max({ _halfSize.x, _halfSize.y, _halfSize.z });
	}

	void Box::setSize(const glm::vec3 size) { setHalfSize(size * 0.5f); }
	
	float Box::radius() const { return _radius; }
	glm::vec3 Box::halfSize() const { return _halfSize; }
	glm::vec3 Box::size() const { return _halfSize * 2.0f; }
	glm::vec3 Box::min() const { return center() - halfSize(); }
	glm::vec3 Box::max() const { return center() + halfSize(); }
}
