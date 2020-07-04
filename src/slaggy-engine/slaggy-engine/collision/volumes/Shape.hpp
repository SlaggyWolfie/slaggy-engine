#pragma once
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <collision/IIntersect.hpp>

#include <glm/glm.hpp>

namespace slaggy
{
	class Transform;

	class Shape : public IIntersect
	{		
	public:		
		virtual glm::vec3 center() const;
		virtual glm::mat4 transformationMatrix() const;
		virtual glm::mat4 scaledTransformationMatrix() const;

		virtual float radius() const = 0;

		virtual bool isInside(glm::vec3 point) const = 0;
		virtual glm::vec3 closestPointTo(glm::vec3 point) const = 0;

		virtual void render(const glm::mat4& view, const glm::mat4& proj) const { }
		virtual void render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const { }
		
	protected:
		virtual Transform* transform() const = 0;
	};
}
#endif
