#pragma once
#ifndef BB_HPP
#define BB_HPP

#include <collision/Shape.hpp>

namespace slaggy
{
	class Collider;
	class World;

	class BB : public Shape
	{
	protected:
		glm::vec3 _halfSize = glm::vec3(0.5f);
		float _radius = 0;
		
	public:

		void setHalfSize(glm::vec3 halfSize);
		void setSize(glm::vec3 size);

		float radius() override;
		glm::vec3 halfSize() override;
		glm::vec3 size() override;
		glm::vec3 min() override;
		glm::vec3 max() override;
	};
}
#endif
