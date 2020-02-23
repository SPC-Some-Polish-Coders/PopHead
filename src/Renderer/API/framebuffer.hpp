#pragma once

namespace ph {

class Framebuffer
{
public:
	void init(unsigned width, unsigned height);
	void remove();

	void onWindowResize(unsigned width, unsigned height);

	void bind();
	void bindTextureColorBuffer(unsigned slot);
	
private:
	unsigned mFramebufferID;
	unsigned mColorBufferTextureID;
	unsigned mRenderBufferID;
};

}
