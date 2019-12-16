#include "vertexBuffer.hpp"
#include "openglErrors.hpp"
#include "Logs/logs.hpp"
#include "GL/glew.h"
#include <array>
#include <Utilities/rect.hpp>

namespace ph { 

void VertexBuffer::init()
{
	GLCheck( glGenBuffers(1, &mID) );
}

void VertexBuffer::remove()
{
	GLCheck( glDeleteBuffers(1, &mID) );
}

void VertexBuffer::bind()
{
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mID) );
}

void VertexBuffer::setData(float* vertices, size_t arraySize, unsigned dataUsage)
{
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mID) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, arraySize, vertices, dataUsage) );
}

}
