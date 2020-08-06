#pragma once
#ifndef BB_HPP
#define BB_HPP

#include <physics/volumes/Shape.hpp>

namespace slaggy
{
	class Box : public Shape
	{		
	public:
		void setHalfSize(glm::vec3 halfSize);
		void setSize(glm::vec3 size);

		float radius() const override;
		virtual glm::vec3 halfSize() const;
		virtual glm::vec3 size() const;
		virtual glm::vec3 min() const;
		virtual glm::vec3 max() const;

		glm::mat4 scaledTransformationMatrix() const override;

		void render(const glm::mat4& view, const glm::mat4& proj) const override;
		void render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const override;
		
	private:
		glm::vec3 _halfSize = glm::vec3(0.5f);
	};
}
#endif
