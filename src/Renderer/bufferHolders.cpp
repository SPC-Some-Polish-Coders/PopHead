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
	mNames.emplace_back(name);
	mReferenceCounters.emplace_back(1);

	// TODO: Use width and height
	std::array<float, 16> vertices = {
		// positions | texture coords
		0.5f,  0.5f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f  // top left 
	};

	VertexBuffer vbo = createVertexBuffer(vertices.data(), vertices.size() * sizeof(float));
	mVertexBuffers.emplace_back(vbo);
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

VertexBuffer IndexBufferHolder::getBuffer(const std::string& name, unsigned numberOfElements, bool thisBufferMightAlreadyExist)
{
	return VertexBuffer();
}

void IndexBufferHolder::deleteBuffer(VertexBuffer)
{
}

}
