#include "buffers.hpp"
#include <GL/glew.h>

namespace ph {

// VERTEX BUFFER

VertexBuffer::VertexBuffer(float* vertices, size_t arraySize)
{
	glGenBuffers(1, &mID);
	glBindBuffer(GL_ARRAY_BUFFER, mID);
	glBufferData(GL_ARRAY_BUFFER, arraySize, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &mID);
}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, mID);
}


// INDEX BUFFER

IndexBuffer::IndexBuffer(unsigned* indices, size_t numberOfIndices)
{
	glGenBuffers(1, &mID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices, indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &mID);
}

void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
}

}
