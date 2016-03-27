/*****************************************************

	hwFrameBufferOGL.cpp
		OpenGL Frame Buffer

*****************************************************/

#ifndef HWFRAMEBUFFER_OGL_H_INCLUDED
#define HWFRAMEBUFFER_OGL_H_INCLUDED

#include "hwRenderBuffer.h"

namespace hgogl {

/**
*	Implements an OpenGL Frame Buffer Object
*/
class HgFrameBufferOGL {

protected:

	GLuint id;

public:

	HgFrameBufferOGL ();
	~HgFrameBufferOGL ();

	void attachRenderBuffer  (HgRenderBufferOGL& buffer, size_t attachPoint=1);
	void attachDepthBuffer   (HgRenderBufferOGL& buffer);
	void attachStencilBuffer (HgRenderBufferOGL& buffer);
	void attachDepthAndStencilBuffer (HgRenderBufferOGL& buffer);
};

}

#endif
