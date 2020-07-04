#include "PointPlane.hpp"

namespace slaggy
{
	float PointPlane::distance() const
	{
		return glm::length(position);
	}
}
