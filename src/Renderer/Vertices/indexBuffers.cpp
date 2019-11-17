#include "indexBuffers.hpp"
#include "Renderer/openglErrors.hpp"
#include "Logs/logs.hpp"
#include "GL/glew.h"
#include <array>

namespace ph {

void IndexBuffer::init()
{
	GLCheck( glGenBuffers(1, &mID) );
}

void IndexBuffer::remove()
{
	GLCheck( glDeleteBuffers(1, &mID) );
}

void IndexBuffer::setData(unsigned* indices, unsigned numberOfIndices)
{
	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID) );
	GLCheck( glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(unsigned), indices, GL_STATIC_DRAW) );
	mNumberOfIndices = numberOfIndices;
}

void IndexBuffer::bind()
{
	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID) );
}

}
