#include "vertexBuffers.hpp"
#include "Renderer/openglErrors.hpp"
#include "Logs/logs.hpp"
#include <array>

namespace ph { 

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
	GLCheck(glDeleteBuffers(1, &vbo.mID));
}

void bind(VertexBuffer vbo)
{
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo.mID));
}

VertexBuffer VertexBufferHolder::getRectangleVertexBuffer(const std::string& name, unsigned width, unsigned height, bool thisBufferMightAlreadyExist)
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

	VertexBuffer vbo = createVertexBuffer(vertices.data(), vertices.size() * sizeof(float));
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
