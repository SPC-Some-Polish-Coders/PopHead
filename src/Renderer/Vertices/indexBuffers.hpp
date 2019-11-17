#pragma once 
 
#include <vector>
#include <string>

namespace ph {

class IndexBuffer
{
public:
	void init();
	void remove();

	void setData(unsigned* indices, unsigned numberOfIndices);

	void bind();

	unsigned getID() const { return mID; }

private:
	unsigned mID;
	int mNumberOfIndices;
};

}
