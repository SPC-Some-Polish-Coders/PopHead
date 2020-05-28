#pragma once

namespace ph {

class Framebuffer
{
public:
	void init(u32 width, u32 height);
	void remove();

	void onWindowResize(u32 width, u32 height);

	void bind();
	void bindTextureColorBuffer(u32 slot);
	
private:
	u32 mFramebufferID;
	u32 mColorBufferTextureID;
	u32 mRenderBufferID;
};

}
