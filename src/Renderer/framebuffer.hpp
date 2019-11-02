#pragma once

namespace ph {

class Framebuffer
{
public:
	Framebuffer(const unsigned width, const unsigned height);
	~Framebuffer();
	void reset(const unsigned width, const unsigned height);

	void bind();
	void bindTextureColorBuffer();
	
	static void bindDefaultFramebuffer();

private:
	unsigned mFramebufferID;
	unsigned mColorBufferTextureID;
	unsigned mRenderBufferID;
};

}
