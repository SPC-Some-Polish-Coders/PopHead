#pragma once

#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include <memory>

namespace ph {

enum class VertexBufferLayout
{
	position2,
	position2_texCoords2,
	position2_color4
};

class VertexArray
{
public:
	void init();
	void remove();

	void bind();

	void setVertexBuffer(VertexBuffer, VertexBufferLayout);
	void setIndexBuffer(IndexBuffer);

	auto getVertexBuffer() -> VertexBuffer& { return mVertexBuffer; }
	auto getIndexBuffer() -> IndexBuffer& { return mIndexBuffer; }

private:
	VertexBuffer mVertexBuffer;
	IndexBuffer mIndexBuffer;
	unsigned mID;
};

}
