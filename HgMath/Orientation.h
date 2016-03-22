/*****************************************************

	orientation.h
		3d orientation

*****************************************************/

#ifndef ORIENTATION_H_INCLUDED
#define ORIENTATION_H_INCLUDED

#include "vector.h"

namespace hgm {

class HgOrientation {

protected:
	/**
	* translation vectors
	*/
	HgVector pos;
	HgVector scale;
	/**
	* rotation vectors
	*/
	HgVector right;
	HgVector up;
	HgVector forward;

public:

	inline const HgVector& getPosition () const {return pos;}
	inline       HgVector& getPosition ()       {return pos;}

	inline const HgVector& getScale () const {return scale;}
	inline       HgVector& getScale ()       {return scale;}
};

} // namespace hgm

#endif
