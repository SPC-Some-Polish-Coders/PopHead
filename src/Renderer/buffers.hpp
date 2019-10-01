#pragma once

namespace ph {

class VertexBuffer
{
public:
	VertexBuffer(float* vertices, size_t arraySize);
	~VertexBuffer();

	void bind();

private:
	unsigned mID;
};

class IndexBuffer
{
public:
	IndexBuffer(unsigned* indices, size_t numberOfIndices);
	~IndexBuffer();

	void bind();

private:
	unsigned mID;
};

}