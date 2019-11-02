#pragma once

#include <SFML/Graphics/Color.hpp>

namespace ph {

class Framebuffer
{
public:
	void init(const unsigned width, const unsigned height);
	void reset(const unsigned width, const unsigned height);
	~Framebuffer();

	void bind();
	void bindTextureColorBuffer();
	
	static void bindDefaultFramebuffer();

private:
	unsigned mFramebufferID;
	unsigned mColorBufferTextureID;
	unsigned mRenderBufferID;
};

}
