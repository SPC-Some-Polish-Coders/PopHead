#pragma once

namespace ph {

class Framebuffer
{
public:
	void init(const unsigned width, const unsigned height);
	void remove();

	void onWindowResize(const unsigned width, const unsigned height);

	void bind();
	void bindTextureColorBuffer(unsigned slot);
	
private:
	unsigned mFramebufferID;
	unsigned mColorBufferTextureID;
	unsigned mRenderBufferID;
};

}
