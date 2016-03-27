/*****************************************************

	texture2d.h
		OpenGL Texture support

*****************************************************/

#ifndef TEXTURE_2D_OGL_H_INCLUDED
#define TEXTURE_2D_OGL_H_INCLUDED

#include "texture.h"

namespace hgogl {

/**
*	OpenGL 2d texture
*/
class HgTextureOGL_2d : public HgTextureOGL {

	void copyTexture ();

public:

	HgTextureOGL_2d () {}
	virtual ~HgTextureOGL_2d () {}

	void upload (hgg::HgImage* p);

	void updateMipMapLevels () {}

	void enable ();
};

}

#endif
