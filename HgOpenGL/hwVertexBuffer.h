/*****************************************************

	vertexBufferOGL.cpp
		OpenGL Vertex buffer

*****************************************************/

#ifndef HWVERTEXBUFFER_OGL_H_INCLUDED
#define HWVERTEXBUFFER_OGL_H_INCLUDED

#include <hgstd/base.h>
#include <hggraphics/hwVertexBuffer.h>
#include "glee.h"

namespace hg {

namespace ogl {

/**
*	OpenGL vertex buffer
*/
class HgVertexBufferOGL : public HgVertexHardwareBuffer {

	GLuint id;

protected:

	void* _lock   (unsigned int offset, unsigned int size);
	void  _unlock ();
	GLenum _getUsageGL ();

public:

	vertexBufferOGL(size_t count, size_t size);
	virtual ~vertexBufferOGL();

	void read (void* data, unsigned int offset, unsigned int size);
	void write (void* data, unsigned int offset, unsigned int size);

	inline GLuint getID () const {return id;}
};

}

}

#endif
