#pragma once

#include <utility>

namespace ph {

// VERTEX BUFFER
struct VertexBuffer { unsigned mID; };
VertexBuffer createVertexBuffer(float* vertices, size_t arraySize);
void deleteVertexBuffer(VertexBuffer);
void bind(VertexBuffer);

// INDEX BUFFER
struct IndexBuffer { unsigned mID; };
IndexBuffer createIndexBuffer(unsigned* indices, size_t arraySize);
void deleteIndexBuffer(IndexBuffer);
void bind(IndexBuffer);

// HELPER BUFFER CREATORS
std::pair<float*, size_t> getRectangleVertexBuffer();
std::pair<unsigned*, size_t> getRectangleIndexBuffer();

}
