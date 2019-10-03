#include "vertexArray.hpp"
#include "openglErrors.hpp"
#include "GL/glew.h"

namespace ph {

VertexArray::VertexArray()
{
	GLCheck( glGenVertexArrays(1, &mID) );
}

VertexArray::~VertexArray()
{
	GLCheck( glDeleteVertexArrays(1, &mID) );

	// NOTE: Maybe should destroy vertex buffer and index buffer
}

void VertexArray::bind()
{
	GLCheck( glBindVertexArray(mID) );
}

void VertexArray::unbind()
{
	GLCheck( glBindVertexArray(0) );
}

void VertexArray::setVertexBuffer(VertexBuffer vbo, VertexBufferLayout layout)
{
	this->bind();
	ph::bind(vbo);
	mVertexBuffer = vbo;

	// TODO: Clean this code or make better layout setting abstraction
	switch(layout)
	{
	case VertexBufferLayout::position2:
		GLCheck( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0) );
		GLCheck( glEnableVertexAttribArray(0) );
		break;
	case VertexBufferLayout::position2_texCoords2:
		GLCheck( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0) );
		GLCheck( glEnableVertexAttribArray(0));
		GLCheck( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))) );
		GLCheck( glEnableVertexAttribArray(1));
		break;
	case VertexBufferLayout::position2_color4:
		GLCheck( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0) );
		GLCheck( glEnableVertexAttribArray(0));
		GLCheck( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float))) );
		GLCheck( glEnableVertexAttribArray(1));
		break;
	default:
		break;
	}
}

void VertexArray::setIndexBuffer(IndexBuffer ibo)
{
	this->bind();
	ph::bind(ibo);
	mIndexBuffer = ibo;
}

}
