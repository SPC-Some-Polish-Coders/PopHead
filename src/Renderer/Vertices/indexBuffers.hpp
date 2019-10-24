#pragma once 
 
#include <vector>
#include <string>

namespace ph {

struct IndexBuffer
{
	unsigned mID;
	int mNumberOfIndices;
};

IndexBuffer createIndexBuffer();
void setData(IndexBuffer&, unsigned* indices, unsigned numberOfIndices);
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

	IndexBuffer addAndGetIndexBuffer(const std::string& name, unsigned* data, unsigned numberOfIndices);
	IndexBuffer getIndexBuffer(const std::string& name);
	void deleteIndexBuffer(IndexBuffer);

	// TODO_ren: Delete this rectangle index buffer, now it'll be hold by Renderer storage
	IndexBuffer getRectangleIndexBuffer() const { return mRectangleIndexBuffer; }

private:
	std::vector<std::string> mNames;
	std::vector<int> mReferenceCounters;
	std::vector<IndexBuffer> mIndexBuffers;

	IndexBuffer mRectangleIndexBuffer;
};

}