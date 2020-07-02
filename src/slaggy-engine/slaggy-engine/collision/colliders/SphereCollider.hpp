#pragma once
#ifndef SPHERE_COLLIDER_HPP
#define SPHERE_COLLIDER_HPP

#include <core/Component.hpp>
#include <collision/volumes/Sphere.hpp>

namespace slaggy
{
	class SphereCollider : public virtual Component, public virtual Sphere
	{
	public:
		Component* clone() override;
		
	protected:
		Transform* transform() const override;
	};
}
#endif