/*****************************************************

	hwRenderBufferOGL.cpp
		OpenGL Render Buffer

*****************************************************/

#ifndef RENDERBUFFER_OGL_H_INCLUDED
#define RENDERBUFFER_OGL_H_INCLUDED

#include "glee.h"

namespace hgogl {

/**
*	Implements an OpenGL Render Buffer Object.
*	Note that this buffer cannot be read or written from;
*	rather it is done indirectly through texture objects or
*	frame buffer objects.
*
*	Call the associative attach method of hwFrameBufferOGL or textureOGL.
*/
class HgRenderBufferOGL {

protected:

	GLuint id;
	size_t width;
	size_t height;
	size_t samples;

	inline GLuint getID() {return id;}

public:

	HgRenderBufferOGL (unsigned int format, size_t width, size_t height, size_t samples=0);
	~HgRenderBufferOGL ();

	inline size_t getWidth () const {return width;}
	inline size_t getHeight() const {return height;}
	inline size_t getSampleCount() const {return samples;}

	friend class HgFrameBufferOGL;
};

} // namespace hgogl

#endif
