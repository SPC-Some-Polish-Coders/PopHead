#pragma once

#include <memory>
#include "buffers.hpp"

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
	VertexArray();
	~VertexArray();

	void bind();
	void unbind();

	void setVertexBuffer(VertexBuffer, VertexBufferLayout);
	void setIndexBuffer(IndexBuffer);

	auto getVertexBuffer() -> const VertexBuffer& { return mVertexBuffer; }
	auto getIndexBuffer() -> const IndexBuffer& { return mIndexBuffer; }

private:
	unsigned mID;
	VertexBuffer mVertexBuffer;
	IndexBuffer mIndexBuffer;
};

}
