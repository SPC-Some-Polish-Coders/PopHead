#include "buffers.hpp"
#include "openglErrors.hpp"
#include <GL/glew.h>
#include <array>

namespace ph {

// VERTEX BUFFER

VertexBuffer createVertexBuffer(float* vertices, size_t arraySize)
{
	unsigned id;
	GLCheck( glGenBuffers(1, &id) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, id) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, arraySize, vertices, GL_STATIC_DRAW) );
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
	GLCheck( glGenBuffers(1, &id) );
	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id) );
	GLCheck( glBufferData(GL_ELEMENT_ARRAY_BUFFER, arraySize, indices, GL_STATIC_DRAW) );
	return {id};
}

void deleteIndexBuffer(IndexBuffer ibo)
{
	glDeleteBuffers( 1, &ibo.mID );
}

void bind(IndexBuffer ibo)
{
	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.mID) );
}


// HELPER BUFFER CREATORS

namespace {
	std::array<float, 16> vertices = {
		// positions | texture coords
		0.5f,  0.5f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f  // top left 
	};

	std::array<unsigned, 6> indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
}

std::pair<float*, size_t> getRectangleVertexBuffer()
{
	return std::pair<float*, size_t>(vertices.data(), vertices.size() * sizeof(float));
}

std::pair<unsigned*, size_t> getRectangleIndexBuffer()
{
	return std::pair<unsigned*, size_t>(indices.data(), indices.size() * sizeof(unsigned));
}

}
