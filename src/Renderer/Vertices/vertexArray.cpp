#include "vertexArray.hpp"
#include "Renderer/openglErrors.hpp"
#include "GL/glew.h"

// TODO_ren: Maybe make VertexArray not hold VertexBuffer and IndexBuffer

namespace ph {

void VertexArray::init()
{
	GLCheck( glGenVertexArrays(1, &mID) );
}

void VertexArray::remove()
{
	GLCheck( glDeleteVertexArrays(1, &mID) );
}

void VertexArray::bind()
{
	GLCheck( glBindVertexArray(mID) );
}

void VertexArray::setVertexBuffer(VertexBuffer vbo, VertexBufferLayout layout)
{
	this->bind();
	vbo.bind();
	mVertexBuffer = vbo;

	// TODO_ren: Support setting multiple VertexBuffers

	// TODO_ren: Clean this code or make better layout setting abstraction
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
	ibo.bind();
	mIndexBuffer = ibo;
}

}
