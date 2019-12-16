#pragma once 
 
#include "glEnums.hpp"
#include "Utilities/rect.hpp"
#include <vector>
#include <string>

namespace ph {

class VertexBuffer
{
public:
	void init();
	void remove();

	void setData(float* vertices, size_t arraySize, DataUsage);
	void setTextureRect(const IntRect& textureRect, const sf::Vector2i textureSize);

	void bind();
	
	unsigned getID() const { return mID; }

private:
	unsigned mID;
};


} 
