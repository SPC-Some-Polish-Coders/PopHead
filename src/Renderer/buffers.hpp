#pragma once

namespace ph {

enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

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