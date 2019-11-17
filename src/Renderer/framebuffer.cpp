#include "framebuffer.hpp"
#include "openglErrors.hpp"
#include "Logs/logs.hpp"
#include <GL/glew.h>

namespace ph {

void Framebuffer::init(const unsigned width, const unsigned height)
{
	GLCheck( glGenFramebuffers(1, &mFramebufferID) );
	GLCheck( glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID) );

	// TODO_ren: Maybe use Texture class here
	GLCheck( glGenTextures(1, &mColorBufferTextureID) );
	GLCheck( glBindTexture(GL_TEXTURE_2D, mColorBufferTextureID) );
	GLCheck( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
	GLCheck( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBufferTextureID, 0) );

	GLCheck( glGenRenderbuffers(1, &mRenderBufferID) );
	GLCheck( glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID) );
	GLCheck( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height) );
	GLCheck( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferID) );

	PH_ASSERT_UNEXPECTED_SITUATION(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");
	
	GLCheck( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
}

void Framebuffer::remove()
{
	GLCheck( glDeleteFramebuffers(1, &mFramebufferID) );
	GLCheck( glDeleteTextures(1, &mColorBufferTextureID) );
	GLCheck( glDeleteRenderbuffers(1, &mRenderBufferID) );
}

void Framebuffer::onWindowResize(const unsigned width, const unsigned height)
{
	GLCheck( glBindTexture(GL_TEXTURE_2D, mColorBufferTextureID) );
	GLCheck( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr) );

	GLCheck( glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID) );
	GLCheck( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height) );
	
	PH_ASSERT_UNEXPECTED_SITUATION(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");
}

void Framebuffer::bind()
{
	GLCheck( glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID) );
}

void Framebuffer::bindTextureColorBuffer(unsigned slot)
{
	GLCheck( glActiveTexture(GL_TEXTURE0 + slot) );
	GLCheck( glBindTexture(GL_TEXTURE_2D, mColorBufferTextureID) );
}

}
