
#include <hggraphics/image.h>
#include "texture2d.h"

using namespace hgg;

namespace hgogl {

/**
*	Copies texture into system-defined texture format
*/
void HgTextureOGL_2d::copyTexture () {
	if (pkImage) {
		glBindTexture (GL_TEXTURE_2D, id);

		// set default type
		GLint internalFormat=GL_RGBA;
		GLenum format=GL_BGRA_EXT;
		GLenum type=GL_UNSIGNED_BYTE;

		// test format
		switch (pkImage->format) {

			// 16bit 555a RGB
			case HG_IMAGE_FORMAT_555aRGBA: {

				internalFormat = GL_RGBA;
				format = GL_BGRA_EXT;
				type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
				break;
			}

			// 16 bit 565 RGB
			case HG_IMAGE_FORMAT_565RGB: {

				internalFormat = GL_RGB;
				format = GL_RGB;
				type = GL_UNSIGNED_SHORT_5_6_5;
				break;
			}

			// 24 bit 888 RGB
  			case HG_IMAGE_FORMAT_888RGB: {

				internalFormat = GL_RGB8;
				format = GL_RGB;
				type = GL_UNSIGNED_BYTE;
				break;
			}

			// most common 32 bit 8888 RGBA
			case HG_IMAGE_FORMAT_8888RGBA : {

				internalFormat=GL_RGBA;
				format=GL_RGBA;
				type=GL_UNSIGNED_INT_8_8_8_8_REV;
				break;
			}
			default:
				return;
		}

		if (mipMaps)
		{
			// generate mipmaps
			glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );

			// enable bilinear mipmap filter
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {

			mipMaps=false;

			// enable bilinear filter without mipmaps
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		// set image data
		glTexImage2D (GL_TEXTURE_2D, 0, internalFormat, pkImage->width,
			pkImage->height, (border) ? 1 : 0, format,
			type, pkImage->data);
	}
}

/**
*	Uploads image into texture
*/
void HgTextureOGL_2d::upload (HgImage* p) {
	if (p) {
		pitch = p->width * p->bpp;
		pkImage = p;
		copyTexture ();
	}
}

/**
*	Enable texture
*/
void HgTextureOGL_2d::enable() {
	if (id)
		glBindTexture (GL_TEXTURE_2D, id);
}

}
