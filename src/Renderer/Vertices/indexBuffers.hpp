#pragma once 
 
#include <vector>
#include <string>

namespace ph {

struct IndexBuffer
{
	unsigned mID;
};

IndexBuffer createIndexBuffer();
void setData(IndexBuffer, unsigned* indices, size_t arraySize);
void deleteIndexBuffer(IndexBuffer);
void bind(IndexBuffer);

class IndexBufferHolder
{
private:
	IndexBufferHolder();

public:
	static IndexBufferHolder& getInstance()
	{
		static IndexBufferHolder globalIndexBufferHolder;
		return globalIndexBufferHolder;
	}

	IndexBufferHolder(IndexBufferHolder&) = delete;
	void operator=(IndexBufferHolder const&) = delete;

	IndexBuffer addAndGetIndexBuffer(const std::string& name, unsigned* data, size_t arraySize);
	IndexBuffer getIndexBuffer(const std::string& name);
	void deleteIndexBuffer(IndexBuffer);

	IndexBuffer getRectangleIndexBuffer() const { return mRectangleIndexBuffer; }

private:
	std::vector<std::string> mNames;
	std::vector<int> mReferenceCounters;
	std::vector<IndexBuffer> mIndexBuffers;

	IndexBuffer mRectangleIndexBuffer;
};

}