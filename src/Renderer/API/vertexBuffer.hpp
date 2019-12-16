#pragma once 
 
#include "Utilities/rect.hpp"
#include <vector>
#include <string>

namespace ph {

class VertexBuffer
{
public:
	void init();
	void remove();

	void setData(float* vertices, size_t arraySize, unsigned dataUsage);

	void bind();
	
	unsigned getID() const { return mID; }

private:
	unsigned mID;
};


} 
