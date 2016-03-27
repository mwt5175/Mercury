/*****************************************************

	hwIndexBufferOGL.h
		OpenGL Index buffer

*****************************************************/

#ifndef HWINDEXBUFFER_OGL_H_INCLUDED
#define HWINDEXBUFFER_OGL_H_INCLUDED

#include <hgstd/base.h>

namespace hgogl {

	class HgIndexBufferOGL : public hgg::HgIndexHardwareBuffer {

	GLuint id;

	GLenum _getUsageGL();

protected:

	void* _lock   (unsigned int offset, unsigned int size);
	void  _unlock ();

public:

	HgIndexBufferOGL(indexType type, uint numIndices);
	virtual ~HgIndexBufferOGL();

	void read (void* data, unsigned int offset, unsigned int size);
	void write (void* data, unsigned int offset, unsigned int size);
};

}

#endif
