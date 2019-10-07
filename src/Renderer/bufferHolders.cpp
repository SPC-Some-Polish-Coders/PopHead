#include "bufferHolders.hpp"
#include "Logs/logs.hpp"
#include <array>
#include <algorithm>

namespace ph {

VertexBuffer VertexBufferHolder::getRectangleVertexBuffer(const std::string& name, unsigned width, unsigned height, bool thisBufferMightAlreadyExist)
{
	// look for existing buffer
	if(thisBufferMightAlreadyExist)
	{
		for(int i = 0; i < mNames.size(); ++i) {
			if(mNames[i] == name) {
				++mReferenceCounters[i];
				return mVertexBuffers[i];
			}
		}
	}

	// create new buffer
	std::array<float, 16> vertices = {
		// positions  texture coords
		1.f, 0.f, 1.0f, 1.0f, // top right
		1.f, 1.f, 1.0f, 0.0f, // bottom right
		0.f, 1.f, 0.0f, 0.0f, // bottom left
		0.f, 0.f, 0.0f, 1.0f  // top left 
	};
	VertexBuffer vbo = createVertexBuffer(vertices.data(), vertices.size() * sizeof(float));
	mVertexBuffers.emplace_back(vbo);
	mNames.emplace_back(name);
	mReferenceCounters.emplace_back(1);
	return vbo;
}

void VertexBufferHolder::deleteBuffer(VertexBuffer vbo)
{
	for(int i = 0; i < mVertexBuffers.size(); ++i) 
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

IndexBuffer IndexBufferHolder::getRectangleIndexBuffer(const std::string& name, bool thisBufferMightAlreadyExist)
{
	// look for existing buffer
	if(thisBufferMightAlreadyExist)
	{
		for(int i = 0; i < mNames.size(); ++i) {
			if(mNames[i] == name) {
				++mReferenceCounters[i];
				return mIndexBuffers[i];
			}
		}
	}

	// create new buffer
	std::array<unsigned, 6> indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	IndexBuffer ibo = createIndexBuffer(indices.data(), indices.size() * sizeof(unsigned));
	mIndexBuffers.emplace_back(ibo);
	mNames.emplace_back(name);
	mReferenceCounters.emplace_back(1);
	return ibo;
}

void IndexBufferHolder::deleteBuffer(IndexBuffer ibo)
{
	for(int i = 0; i < mIndexBuffers.size(); ++i)
	{
		if(mIndexBuffers[i].mID == ibo.mID) {
			--mReferenceCounters[i];
			mNames.erase(mNames.begin() + i);
			mReferenceCounters.erase(mReferenceCounters.begin() + i);
			mIndexBuffers.erase(mIndexBuffers.begin() + i);
			return;
		}
	}
	PH_LOG_WARNING("You're trying to delete index buffer which doesn't exist");
}

}
