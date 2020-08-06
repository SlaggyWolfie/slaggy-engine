#include "Box.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <utils/BoxDebug.hpp>

namespace slaggy
{
	glm::mat4 Box::scaledTransformationMatrix() const
	{
		//return glm::scale(transformationMatrix(), halfSize());
		return glm::scale(transformationMatrix(), size());
	}
	
	void Box::setHalfSize(const glm::vec3 halfSize) { _halfSize = halfSize; }
	void Box::setSize(const glm::vec3 size) { setHalfSize(size * 0.5f); }

	float Box::radius() const { return glm::length(_halfSize);; }
	glm::vec3 Box::halfSize() const { return _halfSize; }
	glm::vec3 Box::size() const { return _halfSize * 2.0f; }
	glm::vec3 Box::min() const { return center() - halfSize(); }
	glm::vec3 Box::max() const { return center() + halfSize(); }

	void Box::render(const glm::mat4& view, const glm::mat4& proj) const
	{
		BoxDebug::instance().render(glm::vec3(0, 1, 1), *this, view, proj);
	}

	void Box::render(const glm::vec3& color, const glm::mat4& view, const glm::mat4& proj) const
	{
		BoxDebug::instance().render(color, *this, view, proj);
	}
}
