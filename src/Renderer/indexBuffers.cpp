#include "indexBuffers.hpp"
#include "openglErrors.hpp"
#include "Logs/logs.hpp"
#include <array>

namespace ph {

IndexBuffer createIndexBuffer(unsigned* indices, size_t arraySize)
{
	unsigned id;
	GLCheck(glGenBuffers(1, &id));
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, arraySize, indices, GL_STATIC_DRAW));
	return {id};
}

void deleteIndexBuffer(IndexBuffer ibo)
{
	glDeleteBuffers(1, &ibo.mID);
}

void bind(IndexBuffer ibo)
{
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.mID));
}

IndexBuffer IndexBufferHolder::getRectangleIndexBuffer(const std::string& name, bool thisBufferMightAlreadyExist)
{
	// look for existing buffer
	if(thisBufferMightAlreadyExist)
	{
		for(size_t i = 0; i < mNames.size(); ++i) {
			if(mNames[i] == name) {
				++mReferenceCounters[i];
				return mIndexBuffers[i];
			}
		}
	}

	// create new buffer
	std::array<unsigned, 6> indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	IndexBuffer ibo = createIndexBuffer(indices.data(), indices.size() * sizeof(unsigned));
	mIndexBuffers.emplace_back(ibo);
	mNames.emplace_back(name);
	mReferenceCounters.emplace_back(1);
	return ibo;
}

void IndexBufferHolder::deleteBuffer(IndexBuffer ibo)
{
	for(size_t i = 0; i < mIndexBuffers.size(); ++i)
	{
		if(mIndexBuffers[i].mID == ibo.mID) {
			--mReferenceCounters[i];
			mNames.erase(mNames.begin() + i);
			mReferenceCounters.erase(mReferenceCounters.begin() + i);
			mIndexBuffers.erase(mIndexBuffers.begin() + i);
			return;
		}
	}
	PH_LOG_WARNING("You're trying to delete index buffer which doesn't exist");
}

}
