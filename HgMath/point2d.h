/*****************************************************

	point2d.h
		2D Point class

*****************************************************/

#ifndef POINT2D_H_INCLUDED
#define POINT2D_H_INCLUDED

namespace hgm {

class HgPoint2d {

public:

	float x;
	float y;

	HgPoint2d () : x(0), y(0) {}
	HgPoint2d (float a, float b) : x(a), y(b) {}

	inline void set (float a, float b) {x=a; y=b;}
	inline void get (float& a, float& b) {a=x; b=y;}

	inline HgPoint2d& operator = (const HgPoint2d& a) {
		x=a.x;
		y=a.y;
		return *this;
	}

	inline HgPoint2d operator+ (HgPoint2d a) {
		HgPoint2d tmp;
		tmp.x = x + a.x;
		tmp.y = y + a.y;
		return tmp;
	}

	inline HgPoint2d& operator += (HgPoint2d a) {
		x+=a.x;
		y+=a.y;
		return *this;
	}

	inline HgPoint2d operator- (HgPoint2d a) {
		HgPoint2d tmp;
		tmp.x = a.x - x;
		tmp.y = a.y - y;
		return tmp;
	}

	inline HgPoint2d& operator -= (HgPoint2d a) {
		x-=a.x;
		y-=a.y;
		return *this;
	}

	friend inline bool operator == (HgPoint2d& a, HgPoint2d& b);
};

inline bool operator == (HgPoint2d& a, HgPoint2d& b) {
	return b.x==a.x && b.y==a.y;
}

} //namespace hgm

#endif
