#pragma once 
 
#include <vector>
#include <string>

namespace ph { 
 
struct VertexBuffer
{
	unsigned mID;
};

VertexBuffer createVertexBuffer(float* vertices, size_t arraySize);
void deleteVertexBuffer(VertexBuffer);
void bind(VertexBuffer);

class VertexBufferHolder
{
public:
	static VertexBufferHolder& getGlobalInstance()
	{
		static VertexBufferHolder globalVertexBufferHolder;
		return globalVertexBufferHolder;
	}

	VertexBuffer getRectangleVertexBuffer(const std::string& name, unsigned width, unsigned height, bool thisBufferMightAlreadyExist = true);
	void deleteBuffer(VertexBuffer);

private:
	std::vector<std::string> mNames;
	std::vector<int> mReferenceCounters;
	std::vector<VertexBuffer> mVertexBuffers;
};

} 
