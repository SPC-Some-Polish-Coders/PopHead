#include "buffers.hpp"
#include "openglErrors.hpp"
#include <GL/glew.h>

namespace ph {

// VERTEX BUFFER

VertexBuffer::VertexBuffer(float* vertices, size_t arraySize)
{
	GLCheck( glGenBuffers(1, &mID) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mID) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, arraySize, vertices, GL_STATIC_DRAW) );
}

VertexBuffer::~VertexBuffer()
{
	GLCheck( glDeleteBuffers(1, &mID) );
}

void VertexBuffer::bind()
{
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mID) );
}


// INDEX BUFFER

IndexBuffer::IndexBuffer(unsigned* indices, size_t numberOfIndices)
{
	GLCheck( glGenBuffers(1, &mID) );
	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID) );
	GLCheck( glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices, indices, GL_STATIC_DRAW) );
}

IndexBuffer::~IndexBuffer()
{
	GLCheck( glDeleteBuffers(1, &mID) );
}

void IndexBuffer::bind()
{
	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID) );
}

}
