/*****************************************************

	frustum.h
		View Frustrum

*****************************************************/

#ifndef FRUSTUM_H_INCLUDED
#define FRUSTUM_H_INCLUDED

namespace hgm {

/**
* Frustum object
*/
class HgFrustum {

	double left;
	double right;
	double bottom;
	double top;
	double nearZ;
	double farZ;

public:

	HgFrustum ()
		: left(0.0), right (0.0), bottom (0.0), top (0.0), nearZ (0.0), farZ (0.0) {

	}

	HgFrustum (double l, double r, double b, double t, double n, double f)
		: left(l), right(r), bottom(b), top(t), nearZ(n), farZ(f) {

	}

	inline const double getLeft   () const {return left;}
	inline const double getRight  () const {return right;}
	inline const double getTop    () const {return top;}
	inline const double getBottom () const {return bottom;}
	inline const double getNear   () const {return nearZ;}
	inline const double getFar    () const {return farZ;}

	/**
	* \brief Test if a point is in the frustum
	*
	* \param x X coordinate
	* \param y Y coordinate
	* \param z Z coordinate
	* \return true if point is in frustum, false if not
	*/
	inline bool isPointInside (float& x, float& y, float& z) {

		if (x > left && x < right && y > bottom && y < top
				&& z > nearZ && z < farZ)
			return true;

		return false;
	}
};

}

#endif
