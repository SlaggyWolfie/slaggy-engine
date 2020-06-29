#pragma once
#ifndef BB_HPP
#define BB_HPP

#include <collision/volumes/Shape.hpp>

namespace slaggy
{
	class Collider;
	class World;

	class Box : public Shape
	{
	protected:
		glm::vec3 _halfSize = glm::vec3(0.5f);
		float _radius = 0;
		
	public:

		void setHalfSize(glm::vec3 halfSize);
		void setSize(glm::vec3 size);

		float radius() const override;
		virtual glm::vec3 halfSize() const;
		virtual glm::vec3 size() const;
		virtual glm::vec3 min() const;
		virtual glm::vec3 max() const;
	};
}
#endif
