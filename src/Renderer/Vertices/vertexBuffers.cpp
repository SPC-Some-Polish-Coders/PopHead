#include "vertexBuffers.hpp"
#include "Renderer/openglErrors.hpp"
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

void VertexBuffer::setData(float* vertices, size_t arraySize, DataUsage usage)
{
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mID) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, arraySize, vertices, toGLEnum(usage)) );
}

// TODO_ren: Delete this method when it's no longer usefull
void VertexBuffer::setTextureRect(const IntRect& r, const sf::Vector2i textureSize)
{
	std::array<float, 16> vertices = {
		// positions                                       t  e  x  t  u  r  e       c  o  o  r  d  s
		1.f, 0.f, (float)(r.left + r.width) / (float)textureSize.x, (float)(textureSize.y - r.top) / (float) textureSize.y, // top right
		1.f, 1.f, (float)(r.left + r.width) / (float)textureSize.x, (float)(textureSize.y - r.top - r.height) / (float) textureSize.y, // bottom right
		0.f, 1.f, (float)(r.left) / (float)textureSize.x          , (float)(textureSize.y - r.top - r.height) / (float) textureSize.y, // bottom left
		0.f, 0.f, (float)(r.left) / (float)textureSize.x          , (float)(textureSize.y - r.top) / (float) textureSize.y  // top left
	};

	setData(vertices.data(), vertices.size() * sizeof(float), DataUsage::dynamicDraw);
}

}
