#include "buffers.hpp"
#include "openglErrors.hpp"
#include <GL/glew.h>

namespace ph {

// VERTEX BUFFER

VertexBuffer createVertexBuffer(float* vertices, size_t arraySize)
{
	unsigned id;
	GLCheck(glGenBuffers(1, &id));
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCheck(glBufferData(GL_ARRAY_BUFFER, arraySize, vertices, GL_STATIC_DRAW));
	return {id};
}

void deleteVertexBuffer(VertexBuffer vbo)
{
	GLCheck( glDeleteBuffers(1, &vbo.mID) );
}

void bind(VertexBuffer vbo)
{
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, vbo.mID) );
}


// INDEX BUFFER

IndexBuffer createIndexBuffer(unsigned* indices, size_t arraySize)
{
	unsigned id;
	GLCheck(glGenBuffers(1, &id));
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, arraySize, indices, GL_STATIC_DRAW));
	return {id};
}

void deleteIndexBuffer(IndexBuffer ibo)
{
	glDeleteBuffers(1, &ibo.mID);
}

void bind(IndexBuffer ibo)
{
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.mID));
}

}
