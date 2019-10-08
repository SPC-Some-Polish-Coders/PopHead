#pragma once 
 
#include <vector>
#include <string>

namespace ph {

struct IndexBuffer
{
	unsigned mID;
};

IndexBuffer createIndexBuffer(unsigned* indices, size_t arraySize);
void deleteIndexBuffer(IndexBuffer);
void bind(IndexBuffer);

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