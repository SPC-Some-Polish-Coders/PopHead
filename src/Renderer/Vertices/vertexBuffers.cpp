#include "vertexBuffers.hpp"
#include "Renderer/openglErrors.hpp"
#include "Logs/logs.hpp"
#include "GL/glew.h"
#include <array>
#include <Utilities/rect.hpp>

namespace ph { 

VertexBuffer createVertexBuffer()
{
	unsigned id;
	GLCheck(glGenBuffers(1, &id));
	return {id};
}

void setData(VertexBuffer vbo, float* vertices, size_t arraySize, unsigned dataUsage)
{
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, vbo.mID) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, arraySize, vertices, dataUsage) );
}

void setTextureRect(const VertexBuffer& vbo, const IntRect& r, const sf::Vector2i textureSize)
{
	std::array<float, 16> vertices = {
		// positions                                       t  e  x  t  u  r  e       c  o  o  r  d  s
		(float)r.width , 0.f               , (float)(r.left + r.width) / (float)textureSize.x, (float)(textureSize.y - r.top) / (float) textureSize.y, // top right
		(float)r.width , ( float) r.height , (float)(r.left + r.width) / (float)textureSize.x, (float)(textureSize.y - r.top - r.height) / (float) textureSize.y, // bottom right
		0.f            , ( float) r.height , (float)(r.left) / (float)textureSize.x          , (float)(textureSize.y - r.top - r.height) / (float) textureSize.y, // bottom left
		0.f            , 0.f               , (float)(r.left) / (float)textureSize.x          , (float)(textureSize.y - r.top) / (float) textureSize.y  // top left
	};

	setData(vbo, vertices.data(), vertices.size() * 16, GL_DYNAMIC_DRAW);
}

void deleteVertexBuffer(VertexBuffer vbo)
{
	GLCheck(glDeleteBuffers(1, &vbo.mID));
}

void bind(VertexBuffer vbo)
{
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo.mID));
}

VertexBufferHolder::VertexBufferHolder()
{
	// allocate memory
	mNames.reserve(100);
	mReferenceCounters.reserve(100);
	mVertexBuffers.reserve(100);
}

VertexBuffer VertexBufferHolder::getRectangleVertexBuffer(const std::string& name, unsigned width, unsigned height, bool isAnimated, bool thisBufferMightAlreadyExist)
{
	// look for existing buffer
	if(thisBufferMightAlreadyExist)
	{
		for(size_t i = 0; i < mNames.size(); ++i) {
			if(mNames[i] == name) {
				++mReferenceCounters[i];
				return mVertexBuffers[i];
			}
		}
	}

	// create new buffer
	float h = static_cast<float>(height);
	float w = static_cast<float>(width);

	std::array<float, 16> vertices = {
		// positions  texture coords
		w  , 0.f , 1.0f, 1.0f, // top right
		w  , h   , 1.0f, 0.0f, // bottom right
		0.f, h   , 0.0f, 0.0f, // bottom left
		0.f, 0.f , 0.0f, 1.0f  // top left 
	};

	VertexBuffer vbo = createVertexBuffer();
	setData(vbo, vertices.data(), vertices.size() * sizeof(float), isAnimated ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	mVertexBuffers.emplace_back(vbo);
	mNames.emplace_back(name);
	mReferenceCounters.emplace_back(1);
	return vbo;
}

void VertexBufferHolder::deleteBuffer(VertexBuffer vbo)
{
	for(size_t i = 0; i < mVertexBuffers.size(); ++i)
	{
		if(mVertexBuffers[i].mID == vbo.mID) {
			--mReferenceCounters[i];
			mNames.erase(mNames.begin() + i);
			mReferenceCounters.erase(mReferenceCounters.begin() + i);
			mVertexBuffers.erase(mVertexBuffers.begin() + i);
			return;
		}
	}
	PH_LOG_WARNING("You're trying to delete vertex buffer which doesn't exist");
}

}  
