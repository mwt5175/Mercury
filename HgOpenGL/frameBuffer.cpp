/*****************************************************

	hwFrameBufferOGL.cpp
		OpenGL Frame Buffer

*****************************************************/

#include "frameBuffer.h"

namespace hgogl {

HgFrameBufferOGL::HgFrameBufferOGL () {

	glGenFramebuffers (1, &id);
}

HgFrameBufferOGL::~HgFrameBufferOGL () {

	glDeleteFramebuffers (1, &id);
}

void HgFrameBufferOGL::attachRenderBuffer (HgRenderBufferOGL& buffer, size_t attachPoint) {
//	if (attachPoint >= GL_MAX_COLOR_ATTACHMENTS)
//		assert ("frameBufferOGL::attachRenderBuffer attachPoint exceeds limit");

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+attachPoint,
		GL_RENDERBUFFER,buffer.getID());
}

void HgFrameBufferOGL::attachDepthBuffer (HgRenderBufferOGL& buffer) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,buffer.getID());
}

void HgFrameBufferOGL::attachStencilBuffer (HgRenderBufferOGL& buffer) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,buffer.getID());
}

void HgFrameBufferOGL::attachDepthAndStencilBuffer (HgRenderBufferOGL& buffer) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,buffer.getID());
}

} // namespace hgogl

//glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
