#pragma once
#ifndef POINT_PLANE_HPP
#define POINT_PLANE_HPP

#include <physics/Plane.hpp>

namespace slaggy
{
	class PointPlane : public Plane
	{
	public:
		glm::vec3 position = glm::vec3(0);
		
		float distance() const override;
	};
}
#endif