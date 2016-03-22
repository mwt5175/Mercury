/*****************************************************

	vector2d.h
		2D Vector

*****************************************************/

#ifndef HgVector_H_INCLUDED
#define HgVector_H_INCLUDED

#include <cmath>

namespace hgm {

class HgVector {
public:
	float x;
	float y;
	float z;
	float w;

	HgVector ()
		: x (0), y (0), z(0),w(0)
	{
	}

	HgVector (const HgVector& v)
		: x (v.x), y(v.y), z(v.z),w(v.w)
	{
	}

	HgVector (const float a,const float b, const float c, float w=1.0f)
		: x (a), y (b), z (c)
	{
	}

	inline void set (const float a, const float b, const float c) {
		x = a;
		y = b;
		z = c;
	}

	inline void get (__out float& a, __out float& b, __out float& c) {
		a = x;
		b = y;
		c = z;
	}

	inline float getX () const {
		return x;
	}

	inline float getY () const {
		return y;
	}

	inline float getZ () const {
		return z;
	}

	inline void setX (const float& a) {
		x = a;
	}

	inline void setY (const float& a) {
		y = a;
	}

	inline void setZ (const float& a) {
		z = a;
	}

	inline void zero () {
		x = y = z = 0;
	}

	/**
	* \brief Multiplies vector by scalar
	*
	* \param scalar scalar value
	* \return resulting vector
	*/
	inline HgVector operator * (const float scaler) {
		return HgVector(x*scaler, y*scaler, z*scaler);
	}

	/**
	* \brief Multiplies this vector by scalar
	*
	* \param scalar scalar value
	* \return resulting vector (this)
	*/
	inline HgVector& operator *= (const float scaler) {
		x*=scaler;
		y*=scaler;
		z*=scaler;
		return *this;
	}

	/**
	* \brief Multiplies vector by vector
	*
	* \param v Vector
	* \return resulting vector
	*/
	inline HgVector operator * (const HgVector& v) {

		HgVector r;
		r.x*=v.x;
		r.y*=v.y;
		r.z*=v.z;
		return r;
	}

	/**
	* \brief Multiplies vector by this vector
	*
	* \param v vector
	* \return resulting vector (this)
	*/
	inline HgVector& operator *= (const HgVector& v) {

		x*=v.x;
		y*=v.y;
		z*=v.z;
		return *this;
	}

	/**
	* \brief Adds two vectors
	*
	* \param v vector
	* \return resulting vector
	*/
	inline HgVector operator + (const HgVector& v) {
		return HgVector(x + v.x, y+v.y, z+v.z);
	}

	/**
	* \brief Adds a vector to this vector
	*
	* \param v vector
	* \return resulting vector (this)
	*/
	inline HgVector& operator += (const HgVector& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	/**
	* \brief Adds a vector to this vector
	*
	* \param v vector
	* \return resulting vector (copy)
	*/
	inline HgVector operator + (const float& v) {
		return HgVector(x+v, y+v, z+v);
	}

	/**
	* \brief Adds vector to this vector
	*
	* \param v vector
	* \return resulting vector (this)
	*/
	inline HgVector& operator += (const float& v) {
		x += v;
		y += v;
		z += v;
		return *this;
	}

	/**
	* \brief Subtract this vector by another
	*
	* \param v vector
	* \return resulting vector
	*/
	inline HgVector operator - (const HgVector& v) {
		return HgVector(x - v.x, y-v.y, z - v.z);
	}

	/**
	* \brief Subtract and assign a vector
	*
	* \param v vector
	* \return resulting vector (this)
	*/
	inline HgVector& operator -= (const HgVector& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	/**
	* \brief Subtract this vector by a scalar
	*
	* \param v scalar
	* \return resulting vector
	*/
	inline HgVector operator - (const float& v) {
		return HgVector(x - v, y-v, z-v);
	}

	/**
	* \brief Subtract and assign a scalar
	*
	* \param v scalar
	* \return resulting vector (this)
	*/
	inline HgVector& operator -= (const float& v) {
		x -= v;
		y -= v;
		z -= v;
		return *this;
	}

	/**
	* \brief Test if two vectors are equal
	*
	* \param v vector
	* \return true if this and v vector are equal, false if not
	*/
	inline bool operator == (const HgVector& v) const {
		return x==v.x && y==v.y && z==v.z;
	}

	/**
	* \brief Test if two vectors are not equal
	*
	* \param v vector
	* \return true if this and v vector are not equal, false if not
	*/
	inline bool operator != (const HgVector& v) const {
		return x!=v.x || y!=v.y || z!=v.z;
	}

	/**
	* \brief Assign this vector to another
	*
	* \param v vector to assign
	* \return this vector
	*/
	inline HgVector& operator = (const HgVector& v) {
		x=v.x;
		y=v.y;
		z=v.z;
		return *this;
	}

	/**
	* \brief Negates this vector
	*
	* \return resulting vector. [x,y,z] elements are negated
	*/
	inline HgVector operator - () {
		return HgVector( -x, -y, -z);
	}

	/**
	* \brief Element access by index
	*
	* \param i index
	* \return [x,y,z] element of vector, 0 if invalid
	*/
	inline float operator [] (const unsigned int i) {

		if (i==0)
			return x;
		if (i==1)
			return y;
		if (i==2)
			return z;

		return 0;
	}

	/**
	* \brief Divides vector by scalar
	*
	* \param scalar scalar value
	* \return resulting vector, or a zero vector if scalar is 0
	*/
	HgVector operator / (const float& scaler) {

		if (scaler) {	// watch for divide by 0

			float t = 1.0f / scaler;
			return HgVector (x * t, y * t, z * t);
		}

		return HgVector (0,0,0);
	}

	/**
	* \brief Divides vector by scalar
	*
	* \param scalar scalar value. Must not be 0
	* \return resulting vector (this)
	*/
	HgVector& operator /= (const float& scaler) {

		if (scaler) {	// watch for divide by 0

			float t = 1.0f / scaler;
			x *= t;
			y *= t;
			z *= t;
		}

		return *this;
	}

	/**
	* \brief Returns length of vector
	*
	* \return vector length
	*/
	inline float getLength () const {

		return sqrt ( (x*x) + (y*y) + (z*z) );
	}

	/**
	* \brief Normalize this vector
	*
	* \return this vector
	*/
	inline HgVector& normalize () {

		float l = getLength ();
		if (l==0)
			return *this;

		l = 1.0f / l;
		x *= l;
		y *= l;
		z *= l;

		return *this;
	}

	/**
	* \brief Calculate distance between two vectors
	*
	* \param v vector
	* \return distance
	*/
	inline float distance (const HgVector& v) {

		float dx = x - v.x;
		float dy = y - v.y;
		float dz = z - v.z;

		return (dx * dx) + (dy * dy) + (dz * dz);
	}

	/**
	* \brief Calculate dot product
	*
	* \param v vector
	* \return dot product of vector v
	*/
	inline float dotProduct (const HgVector& v) {
		return x*v.x + y*v.y + z*v.z;
	}

	/**
	* \brief Calculate cross product
	*
	* \param v vector
	* \return resulting vector
	*/
	HgVector crossProduct (const HgVector& v) {

		HgVector tmp = HgVector(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
			);

		return tmp;
	}
};

} // namespace hgm

#endif
