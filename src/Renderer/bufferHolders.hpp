#pragma once

#include "buffers.hpp"
#include <vector>
#include <string>

namespace ph {

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

class IndexBufferHolder
{
public:
	static IndexBufferHolder& getGlobalInstance()
	{
		static IndexBufferHolder globalIndexBufferHolder;
		return globalIndexBufferHolder;
	}

	IndexBuffer getRectangleIndexBuffer(const std::string& name, bool thisBufferMightAlreadyExist = true);
	void deleteBuffer(IndexBuffer);

private:
	std::vector<std::string> mNames;
	std::vector<int> mReferenceCounters;
	std::vector<IndexBuffer > mIndexBuffers;
};

}
