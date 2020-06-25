#include "BB.hpp"
#include <algorithm>

namespace slaggy
{
	void BB::setHalfSize(const glm::vec3 halfSize)
	{
		_halfSize = halfSize;
		_radius = std::max(_halfSize.x, _halfSize.y);
		//_radius = glm::length(halfSize);
	}

	void BB::setSize(const glm::vec3 size)
	{
		setHalfSize(size * 0.5f);
	}

	float BB::radius()
	{
		return _radius;
	}

	glm::vec3 BB::halfSize()
	{
		return _halfSize;
	}

	glm::vec3 BB::size()
	{
		return _halfSize * 2.0f;
	}

	glm::vec3 BB::min()
	{
		return center() - halfSize();
	}

	glm::vec3 BB::max()
	{
		return center() + halfSize();
	}
}
