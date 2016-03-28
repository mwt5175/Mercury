/*****************************************************

	color.h
		Colors

*****************************************************/

#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include <hgstd/base.h>

/**
* \brief Builds up a RGBx 5.5.5.x 16 bit color
*
*/
#define RGB16BIT555(r,g,b) ((r&31) + ((g&31) << 5) ((b&31) << 10))

/**
* \brief Builds up a RGB 5.6.5 16 bit color
*
*/
#define RGB16BIT565(r,g,b) (r / (g << 6) / (b << 11))

/**
* \namespace mercury
*
*/
namespace hgg {

/**
* \brief A color object
*
*/
class HgColor {

public:

	//! represented as a 32 bit color type: BBGGRRAA
	unsigned int	col;

	/**
	* \brief Constructor
	*
	*/
	HgColor ()
		: col (0) {
	}

	/**
	* \brief Constructor
	*
	* \param c Color value
	*/
	HgColor (const int c)
		: col (c) {
	}

	/**
	* \brief Constructor
	*
	* \param r Red component
	* \param g Green component
	* \param b Blue component
	* \param a Alpha component. Defaults to 0
	*/
	HgColor (const int r,const int g,const int b,const int a = 0)
		: col ( ( (a & 0xff) << 24) | ( (r & 0xff) << 16) | ( (g & 0xff) << 8) | (b & 0xff) ) {
	}

	/**
	* \brief Set blue component
	*
	* \param b Blue component
	*/
	inline void setBlue (const int b) {

		col = (b & 0xff) | (col & 0xffffff00);
	}

	/**
	* \brief Set green component
	*
	* \param g Green component
	*/
	inline void setGreen (const int g) {

		col = ((g & 0xff)<<8) | (col & 0xffff00ff);
	}

	/**
	* \brief Set red component
	*
	* \param r Red component
	*/
	inline void setRed (const int r) {

		col = ((r & 0xff)<<16) | (col & 0xff00ffff);
	}

	/**
	* \brief Set alpha component
	*
	* \param a Alpha component
	*/
	inline void setAlpha (const int a) {

		col = ((a & 0xff)<<24) | (col & 0x00ffffff);
	}

	/**
	* \brief Sets color
	*
	* \param r Red component
	* \param g Green component
	* \param b Blue component
	* \param a Alpha component. Defaults to 0
	*/
	inline void set (const int r,const int g,const int b,const int a=0) {

		col = ( ( (a & 0xff) << 24) | ( (r & 0xff) << 16) | ( (g & 0xff) << 8) | (b & 0xff) );
	}

	/**
	* \brief Sets color
	*
	* \param c Color value
	*/
	inline void set (const int c) {
		col = c;
	}

	/**
	* \brief Return blue component
	*
	* \return blue component
	*/
	inline const int getBlue () const {
		return col & 0xff;
	}

	/**
	* \brief Return green component
	*
	* \return green component
	*/
	inline const int getGreen () const {
		return (col >> 8) & 0xff;
	}

	/**
	* \brief Return red component
	*
	* \return red component
	*/
	inline const int getRed () const {
		return (col>> 16) & 0xff;
	}

	/**
	* \brief Return alpha component
	*
	* \return alpha component
	*/
	inline const int getAlpha () const {
		int p = (col >> 24) & 0xff; 
		return p;
	}

	/**
	* \brief Return color
	*
	* \param r Out red component
	* \param g Out green component
	* \param b Out blue component
	* \param a Out alpha component. Defaults to 0
	*/
	inline void get (__out int &r,__out int &g,__out int &b,__out int &a) {

		a = getAlpha ();
		r = getRed ();
		g = getGreen ();
		b = getBlue ();
	}

	/**
	* \brief Return color
	*
	* \return color
	*/
	inline const int get () const {
		return col;
	}

	/**
	* \brief Swaps red and green components for OpenGL
	*
	* \return color
	*/
	inline const int toOpenGLColor () {
		int c = 0;
		c = ( ( getAlpha () << 24) | ( getBlue() << 16) | ( getGreen() << 8) | getRed() );
		return c;
	}

	/**
	* \brief Calculates average color of color components
	*
	* \return average
	*/
	inline const int average () const {
		return (getRed() + getGreen() + getBlue()) / 3;
	}

	/**
	* \brief Assign a color
	*
	* \param c color to assign
	* \return *this
	*/
	inline HgColor& operator = (const HgColor& c) {
		col = c.col;
		return *this;
	}

	/**
	* \brief Assign a color
	*
	* \param c color to assign
	* \return *this
	*/
	inline HgColor& operator = (const int c) {
		col = c;
		return *this;
	}

	/**
	* \brief Test if two colors are equal
	*
	* \param c color to test
	* \return true if equal, false if not
	*/
	inline bool operator == (const HgColor& c) const {

		return col == c.col;
	}

	/**
	* \brief Test if two colors are equal
	*
	* \param c color
	* \return true if equal, false if not
	*/
	inline bool operator == (const unsigned int c) const {

		return col == c;
	}
};

/**
* \brief Float color object
*
*/
class HgColorf {

public:

	//! red component
	float	red;

	//! green component
	float	green;

	//! blue component
	float	blue;

	//! alpha component
	float	alpha;

	/**
	* \brief Constructor
	*
	*/
	HgColorf ()
		: red (0.0f), green (0.0f), blue (0.0f), alpha (0.0f) {
	}

	/**
	* \brief Constructor
	*
	* \param r Out red component
	* \param g Out green component
	* \param b Out blue component
	* \param a Out alpha component. Defaults to 0
	*/
	HgColorf (const float r, const float g,const float b,const float a = 0)
		: red(r), green(g), blue(b), alpha(a) {

	}

	/**
	* \brief Constructor
	*
	* \param c Color
	*/
	HgColorf (const HgColor& c) {

		const float inv = 1.0f / 255.0f;
		red = c.getRed () * inv;
		green = c.getGreen () * inv;
		blue = c.getBlue () * inv;
		alpha = c.getAlpha () * inv;

	}

	/**
	* \brief Converts colorf to color
	*
	* \return color object
	*/
	inline HgColor toColor () {

		return HgColor ( (int) (red*255.0f), (int) (green*255.0f), (int) (blue*255.0f), (int) (alpha*255.0f) );
	}

	/**
	* \brief Converts color to colorf object
	*
	* \param color object
	*/
	inline void fromColor (const HgColor& c) {

		const float inv = 1.0f / 255.0f;
		red = c.getRed () * inv;
		green = c.getGreen () * inv;
		blue = c.getBlue () * inv;
		alpha = c.getAlpha () * inv;
	}

	/**
	* \brief Returns red component
	*
	* \return red component
	*/
	inline const float getRed () const {
		return red;
	}

	/**
	* \brief Returns green component
	*
	* \return green component
	*/
	inline const float getGreen () const {
		return green;
	}

	/**
	* \brief Returns blue component
	*
	* \return blue component
	*/
	inline const float getBlue () const {
		return blue;
	}

	/**
	* \brief Returns alpha component
	*
	* \return alpha component
	*/
	inline const float getAlpha () const {
		return alpha;
	}

	/**
	* \brief Set red component
	*
	* \param red component
	*/
	inline void setRed (const float& r) {
		red = r;
	}

	/**
	* \brief Set green component
	*
	* \param green component
	*/
	inline void setGreen ( const float& g) {
		green = g;
	}

	/**
	* \brief Set blue component
	*
	* \param blue component
	*/
	inline void setBlue ( const float& b) {
		blue = b;
	}

	/**
	* \brief Set alpha component
	*
	* \param alpha component
	*/
	inline void setAlpha ( const float& a) {
		alpha = a;
	}

	/**
	* \brief Set color
	*
	* \param r red component
	* \param g green component
	* \param b blue component
	* \param a alpha component. Defaults to 0
	*/
	inline void set (float r=0.0f, float g=0.0f, float b=0.0f, float a=0.0f) {

		red = r;
		green = g;
		blue = b;
		alpha = a;
	}

	/**
	* \brief Set color
	*
	* \param r Out red component
	* \param g Out green component
	* \param b Out blue component
	* \param a Out alpha component. Defaults to 0
	*/
	inline void get (__out float &r,__out float &g,__out float &b,__out float &a) {

		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	/**
	* \brief Assign color
	*
	* \param c color to assign
	* \return *this
	*/
	HgColorf& operator = (HgColorf c) {

		set (c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha() );
		return *this;
	}

	/**
	* \brief Test if two colors are equal
	*
	* \param c color to test
	* \return true if equal, false if not
	*/
	bool operator == (HgColorf c) {

		return (c.getRed() == red && c.getGreen() == green && c.getBlue() == blue && c.getAlpha() == alpha);
	}
};

}

#endif
