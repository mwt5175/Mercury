/*****************************************************

	hwRenderBufferOGL.cpp
		OpenGL Render Buffer

*****************************************************/

#include "hwRenderBuffer.h"

namespace hgogl {

HgRenderBufferOGL::HgRenderBufferOGL (unsigned int format, size_t width, size_t height, size_t samples) {

//	if (samples > GL_MAX_SAMPLES)
//		assert ("renderBufferOGL::renderBufferOGL Sample size to big");

	glGenRenderbuffers (1, &id);
	glBindRenderbuffer (GL_RENDERBUFFER, id);

	// format must be valid OGL texture image format
	if (!samples)
		glRenderbufferStorage (GL_RENDERBUFFER, format, width, height);
	else
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
}

HgRenderBufferOGL::~HgRenderBufferOGL () {

	glDeleteRenderbuffers(1, &id);
}

}
