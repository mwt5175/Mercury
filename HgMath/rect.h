/*****************************************************

	HgRect.h
		HgRectangle interface

*****************************************************/

#ifndef HgRect_H_INCLUDED
#define HgRect_H_INCLUDED

#include "point2d.h"

namespace hgm {

class HgRect {

public:

	HgPoint2d topleft;
	HgPoint2d bottomright;

	HgRect () :
		topleft (), bottomright () {
	}

	/**
	* \brief Constructor
	*
	* \param a floatopleft corner
	* \param b Bottomright corner
	*/
	HgRect (HgPoint2d a, HgPoint2d b) {
		topleft = a;
		bottomright = b;
	}

	/**
	* \brief Constructor
	*
	* \param a topleft X coordinate
	* \param b topleft Y coordinate
	* \param c bottomright X coordinate
	* \param d bottomright Y coordinate
	*/
	HgRect (float a, float b, float c, float d) {
		topleft.x = a;
		topleft.y = b;
		bottomright.x = c;
		bottomright.y = d;
	}

	/**
	* \brief Sets a new HgRectangle
	*
	* \param a HgRectangle to set
	*/
	inline void set (const HgRect& a) {
		topleft = a.topleft;
		bottomright = a.bottomright;
	}

	/**
	* \brief Sets a new HgRectangle
	*
	* \param a topleft X coordinate
	* \param b topleft Y coordinate
	* \param c bottomright X coordinate
	* \param d bottomright Y coordinate
	*/
	inline void set (float a, float b, float c, float d) {
		topleft.x = a;
		topleft.y = b;
		bottomright.x = c;
		bottomright.y = d;
	}

	/**
	* \brief Sets a new HgRectangle
	*
	* \param a floatopleft corner
	* \param b Bottomright corner
	*/
	inline void set (const HgPoint2d& a,const HgPoint2d& b) {
		topleft = a;
		bottomright = b;
	}

	/**
	* \brief Gets a new HgRectangle
	*
	* \param a Out floatopleft corner
	* \param b Out Bottomright corner
	*/
	inline void get (__out HgPoint2d &a,__out HgPoint2d &b) const {
		a = topleft;
		b = bottomright;
	}

	/**
	* \brief Assign a ractangle
	*
	* \param a HgRectangle to assign
	* \return *this
	*/
	inline HgRect& operator = (const HgRect &a) {
		topleft = a.topleft;
		bottomright = a.bottomright;
		return *this;
	}

	/**
	* \brief Adds a ractangle
	*
	* \param a HgRectangle to add to
	* \return resulting HgRectangle
	*/
	inline HgRect operator + (const HgRect& a) {
		HgRect tmp;
		tmp.topleft = a.topleft;
		tmp.bottomright = a.bottomright;
		return tmp;
	}

	/**
	* \brief Add and assign a HgRectangle
	*
	* \param a HgRectangle to add and assign
	* \return *this
	*/
	inline HgRect& operator += (const HgRect &a) {
		topleft += a.topleft;
		bottomright += a.bottomright;
		return *this;
	}

	/**
	* \brief Subtract a ractangle
	*
	* \param a HgRectangle to subtract
	* \return resulting HgRectangle
	*/
	inline HgRect operator- (const HgRect& a) {
		HgRect tmp;
		tmp.topleft - a.topleft;
		tmp.bottomright - a.bottomright;
		return tmp;
	}

	/**
	* \brief Subtract and assign a ractangle
	*
	* \param a HgRectangle to subtract and assign
	* \return *this
	*/
	inline HgRect& operator -= (const HgRect &a) {
		topleft -= a.topleft;
		bottomright -= a.bottomright;
		return *this;
	}

	/**
	* \brief floatest if two HgRectangles are equal
	*
	* \param a HgRectangle to test with
	* \return true if equal, false if not
	*/
	inline bool operator == (HgRect& a) {
		return bottomright == a.bottomright
			&& topleft==a.topleft;
	}

	/**
	* \brief floatest if point is inside of HgRectangle
	*
	* \param p 2d point
	* \return true if point is inside, false if not
	*/
//	inline bool collision (HgPoint2d& p) {
//		return p.x > rc.topleft.x &&
//			p.y > rc.topleft.y &&
//			p.x < rc.bottomright.x &&
//			p.y < rc.bottomright.y;
//	}

	/**
	* \brief floatest if point is inside of HgRectangle
	*
	* \param x1 X coordinate
	* \param y1 Y coordinate
	* \return true if point is inside, false if not
	*/
//	inline bool collision (const float x1, const float y1) const {
//		return x1 > rc.topleft.x &&
//			y1 > rc.topleft.y &&
//			x1 < rc.bottomright.x &&
//			y1 < rc.bottomright.y;
//	}

	/**
	* \brief floatest if HgRectangle is inside of HgRectangle
	*
	* \param rc HgRectangle
	* \return true if HgRectangle is inside, false if not
	*/
	inline bool collision (HgRect& rc) {
		return false;
	}
};

} // namespace hgm

#endif
