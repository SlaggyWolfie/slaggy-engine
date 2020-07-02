#include "SphereDebug.hpp"

#include <engine/Shader.hpp>
#include <collision/volumes/Sphere.hpp>

#include <utils/Icosphere.h>

namespace slaggy
{
	std::unique_ptr<SphereDebug> SphereDebug::_instance = nullptr;
	
	SphereDebug& SphereDebug::instance()
	{
		if (!_instance) _instance = std::make_unique<SphereDebug>();
		return *_instance;
	}

	SphereDebug::SphereDebug()
	{
		//sphere.setRadius(10);
		sphere = Icosphere(0.5f, 0, false);		
		
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ebo);

		glBindVertexArray(_vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);           // for vertex data
		glBufferData(GL_ARRAY_BUFFER,                   // target
			sphere.getInterleavedVertexSize(), // data size, # of bytes
			sphere.getInterleavedVertices(),   // ptr to vertex data
			GL_STATIC_DRAW);                   // usage

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);   // for index data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
			sphere.getIndexSize(),             // data size, # of bytes
			sphere.getIndices(),               // ptr to index data
			GL_STATIC_DRAW);                   // usage

		// draw icosphere using VBO and GLSL
		const int stride = sphere.getInterleavedStride();     // should be 32 bytes
		glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, (void*)(sizeof(float) * 6));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
				
		// clean
		glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		_shader = std::make_unique<Shader>("shaders/singleColor");
	}

	SphereDebug::~SphereDebug()
	{
		glDeleteVertexArrays(1, &_vao);
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
	}

	unsigned SphereDebug::vao() const
	{
		return _vao;
	}

	void SphereDebug::render(const glm::vec3& color, const glm::mat4& model, const glm::mat4& view,
		const glm::mat4& proj) const
	{
		
		_shader->use();
		_shader->set("color", color);
		_shader->set("mvp", proj * view * model);

		glBindVertexArray(_vao);		
		//glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
		glDrawElements(GL_LINE_STRIP, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
		
		//sphere.draw();
	}

	void SphereDebug::render(const glm::vec3& color, const Sphere& sphere, const glm::mat4& view, const glm::mat4& proj) const
	{
		render(color, sphere.scaledTransformationMatrix(), view, proj);
	}
}
