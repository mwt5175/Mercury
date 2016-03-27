/*****************************************************

	texture.cpp
		OpenGL Texture support

*****************************************************/

#ifndef TEXTURE_OGL_H_INCLUDED
#define TEXTURE_OGL_H_INCLUDED

#include <hggraphics/texture.h>
#include "glee.h"

namespace hgogl {

/**
*	OpenGL texture
*/
class HgTextureOGL : public hgg::HgTexture {

protected:

	GLuint id;

public:

	HgTextureOGL ();
	virtual ~HgTextureOGL ();

	virtual void upload (hgg::HgImage* p) = 0;

	virtual void updateMipMapLevels () = 0;

	virtual void enable () = 0;
};

}

#endif
