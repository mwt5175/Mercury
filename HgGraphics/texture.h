/*****************************************************

	texture.h
		Texture

*****************************************************/

#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include "image.h"

namespace hgg {

enum HgTextureFlags {

	// Tells renderer to generate texture mipmaps
	TEXTURE_GEN_MIPMAPS=0x4000000,

	// Clamps S coordinates
	TEXTURE_WRAP_S = 0x8000000,

	// Clamps T coordinates
	TEXTURE_WRAP_T = 0x10000000,

	// Clamps R coordinates
	TEXTURE_WRAP_R = 0x20000000,

	// Clamps texture if needed
	TEXTURE_CLAMP  = 0x40000000,

	// Repeats texture if needed
	TEXTURE_REPEAT = 0x80000000
};

/**
* Texture coordinate
*/
class HgTextureCoord {

public:

	float u;

	float v;

	HgTextureCoord (float u1=0, float v1=0) : u(u1), v(v1) {}
};

/**
* Texture object
*/
class HgTexture {

protected:

	unsigned int   id;
	HgImage*       pkImage;
	unsigned int   pitch;
	unsigned int   ratio;
	bool           border;
	bool           mipMaps;
	HgTextureFlags flags;

	inline void release () {
		if(pkImage) {
			delete pkImage;
			pkImage=0;
		}
	}

public:

	HgTexture (HgImage* p = 0, bool Border=false)
		: pkImage (p), pitch(0), ratio(0), id(0), border(Border){

		mipMaps=true;
	}

	virtual ~HgTexture () {
		release ();
	}

	inline unsigned int   getId     ()  const {return id;}
	inline int            getPitch  ()  const {return pitch;}
	inline int            getRatio  ()  const {return ratio;}
	inline bool           hasBorder ()  const {return border;}
	inline HgImage*       getImage  ()        {return pkImage;}
	inline HgTextureFlags getFlags  ()        {return flags;}

	inline void upload (HgImage* p) {
		release ();
		pkImage = p;
	}

	/**
	* \brief Set texture flags
	*
	* \param f New flags
	*/
	inline void setFlags (HgTextureFlags f) {
		flags=f;
	}

	/**
	* \brief Returns mipmap state
	*
	* \return true if mipmaps are generated, false if not
	*/
	inline bool hasMipmaps () {
		return mipMaps;
	}

	/**
	* \brief Updates mipmap levels
	*
	*/
	virtual void updateMipMapLevels () {
		mipMaps=false;
	}

	/**
	* \brief Returns texture width
	*
	* \return texture width, 0 if error
	*/
	inline int getWidth () const {

		if (pkImage)
			return pkImage->width;

		return 0;
	}

	/**
	* \brief Returns texture height
	*
	* \return texture height, 0 if error
	*/
	inline int getHeight () const {

		if (pkImage)
			return pkImage->height;

		return 0;
	}

	/**
	* \brief Returns texture format
	*
	* \return texture format, 0 if error
	*/
	inline HgImageFormat getFormat () const {

		if (pkImage)
			return pkImage->format;

		return HG_IMAGE_FORMAT_INVALID;
	}

	/**
	* \brief Compare texture IDs
	*
	* \param p Texture to compare with
	* \return true if this texture ID is less then p, false if not
	*/
	inline bool operator < (HgTexture* p) const {
	
		if (p)
			return p->getId() < id;

		return false;
	}

	/**
	* \brief Compare texture IDs
	*
	* \param p Texture to compare with
	* \return true if this texture ID is greater then p, false if not
	*/
	inline bool operator > (HgTexture* p) const {
	
		if (p)
			return p->getId() > id;

		return false;
	}

	/**
	* \brief Compare texture IDs
	*
	* \param p Texture to compare with
	* \return true if this texture and p IDs are equal, false if not
	*/
	inline bool operator == (HgTexture* p) const {
	
		if (p)
			return p->getId() == id;

		return false;
	}

	/**
	* \brief Compare texture IDs
	*
	* \param p Texture to compare with
	* \return true if this texture and p IDs are not equal, false if they are
	*/
	inline bool operator != (HgTexture* p) const {
	
		if (p)
			return p->getId() != id;

		return false;
	}
};

} // namespace hgg

#endif
