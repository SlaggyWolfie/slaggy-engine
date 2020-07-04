#pragma once
#ifndef SUPER_PLANE_HPP
#define SUPER_PLANE_HPP

#include <collision/Plane.hpp>

namespace slaggy
{
	class SuperPlane : public Plane
	{
	public:
		float position;
		
		float distance() const override;
	};
}
#endif