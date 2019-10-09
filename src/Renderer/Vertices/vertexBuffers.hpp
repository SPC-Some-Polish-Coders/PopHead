#pragma once 
 
#include <Utilities/rect.hpp>
#include <vector>
#include <string>

namespace ph {

struct VertexBuffer
{
	unsigned mID;
};

VertexBuffer createVertexBuffer();
void setData(VertexBuffer, float* vertices, size_t arraySize, unsigned dataUsage);
void setTextureRect(const VertexBuffer& vbo, const IntRect& r, const sf::Vector2i textureSize);
void deleteVertexBuffer(VertexBuffer);
void bind(VertexBuffer);

class VertexBufferHolder
{
private:
	VertexBufferHolder();

public:
	static VertexBufferHolder& getInstance()
	{
		static VertexBufferHolder globalVertexBufferHolder;
		return globalVertexBufferHolder;
	}

	VertexBufferHolder(VertexBufferHolder&) = delete;
	void operator=(VertexBufferHolder const&) = delete;

	VertexBuffer getRectangleVertexBuffer(const std::string& name, unsigned width, unsigned height, bool isAnimated, bool thisBufferMightAlreadyExist = true);
	void deleteBuffer(VertexBuffer);

private:
	std::vector<std::string> mNames;
	std::vector<int> mReferenceCounters;
	std::vector<VertexBuffer> mVertexBuffers;
};

} 
