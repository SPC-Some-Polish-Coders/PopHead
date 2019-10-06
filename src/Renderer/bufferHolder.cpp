#include "bufferHolders.hpp"
#include <array>

namespace ph {

VertexBuffer VertexBufferHolder::getBuffer(const std::string& name, unsigned width, unsigned height, bool thisBufferMightAlreadyExist)
{
	if(thisBufferMightAlreadyExist)
	{
		for(int i = 0; i < mNames.size(); ++i) {
			if(mNames[i] == name) {
				++mReferenceCounters[i];
				return mBuffers[i];
			}
		}
	}

	mNames.emplace_back(name);
	mReferenceCounters.emplace_back(1);
	std::array<

		return BufferType();
}

void VertexBufferHolder::deleteBuffer(VertexBuffer)
{
}

VertexBuffer IndexBufferHolder::getBuffer(const std::string& name, unsigned numberOfElements, bool thisBufferMightAlreadyExist)
{
	return VertexBuffer();
}

void IndexBufferHolder::deleteBuffer(VertexBuffer)
{
}

}
