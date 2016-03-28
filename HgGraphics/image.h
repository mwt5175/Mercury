/*****************************************************

	image.h
		Image interface

*****************************************************/

#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

namespace hgg {

/**
* Image color formats
*/
enum HgImageFormat {
	HG_IMAGE_FORMAT_MONO,
	HG_IMAGE_FORMAT_4INDEX,
	HG_IMAGE_FORMAT_8INDEX,
	HG_IMAGE_FORMAT_565RGB,
	HG_IMAGE_FORMAT_555RGB,
	HG_IMAGE_FORMAT_555aRGBA,
	HG_IMAGE_FORMAT_888RGB,
	HG_IMAGE_FORMAT_8888RGBA,
	HG_IMAGE_COMPRESSED_RGBA_S3TC_DXT1,
	HG_IMAGE_COMPRESSED_RGBA_S3TC_DXT3,
	HG_IMAGE_COMPRESSED_RGBA_S3TC_DXT5,
	HG_IMAGE_FORMAT_INVALID
};

class HgImage {

public:

	HgImageFormat  format;
	unsigned int   width;
	unsigned int   height;
	unsigned int   bpp;
	unsigned int   imageSize;
	unsigned char* data;
	unsigned int   mipMapLevel;

	HgImage () : format (HG_IMAGE_FORMAT_8888RGBA),
		width(0),height(0),bpp (0),imageSize (0),data(0),mipMapLevel(0) { }

	HgImage (int iWidth,int iHeight, int iBpp, HgImageFormat Format = HG_IMAGE_FORMAT_8888RGBA)
		: width(iWidth), height(iHeight),bpp(iBpp),format(Format),imageSize (width*height* (bpp/8) ),data(0) {}

	virtual ~HgImage () {
		if (data) {
			delete [] data;
			data = 0;
		}
	}
};

} // namespace hgg

#endif
