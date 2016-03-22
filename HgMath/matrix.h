/*****************************************************

	matrix.h
		Matrix APIs

*****************************************************/

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "vector.h"

namespace hgm {

/**
* \brief 4x4 matrix
*
* This class describes a 4x4 matrix in the format:
* m[0] m[4] m[8]  m[12]
* m[1] m[5] m[9]  m[13]
* m[2] m[6] m[10] m[14]
* m[3] m[7] m[11] m[15]
*/
template <class T>
class matrix4 {

public:

	union {

		//! matrix
		T m[16];

		//! matrix
		struct  {

			T m11, m12, m13, m14,
			  m21, m22, m23, m24,
			  m31, m32, m33, m34,
			  m41, m42, m43, m44;
		};
	};

	/**
	* \brief constructor
	*/
	matrix4 () {

		//! default to idenitity matrix
		identity ();
	}

	/**
	* \brief constructor
	*
	* \parm m0...m15 matrix element value
	*/
	matrix4 (T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7, T m8, T m9,
		T m10, T m11, T m12, T m13, T m14, T m15) {

		m[0]=m0;
		m[1]=m1;
		m[2]=m2;
		m[3]=m3;
		m[4]=m4;
		m[5]=m5;
		m[6]=m6;
		m[7]=m7;
		m[8]=m8;
		m[9]=m9;
		m[10]=m10;
		m[11]=m11;
		m[12]=m12;
		m[13]=m13;
		m[14]=m14;
		m[15]=m15;
	}

	/**
	* \brief Sets idenitity matrix
	*/
	inline void identity () {

		for (int i = 0; i < 16; i++)
			m[i] = 0;

		m[0] = m[5] = m[10] = m[15] = (T)1;
	}

	/**
	* \brief Test if this is an idenitity matrix
	*
	* \return true if idenitity, false if not
	*/
	inline bool isIdenitity () {

		if (m[0] == 1 && m[5] == 1 && m[10] == 1 && m[15] == 1)
			return true;

		return false;
	}

	/**
	* \brief Returns element at row, column
	*
	* \param row Element row requested
	* \param col Element column requested
	* \return Reference to matrix element
	*/
	inline T& operator () (int row, int col) {
		return m [col * 4 + row];
	}

	/**
	* \brief Returns element at row, column
	*
	* \param row Element row requested
	* \param col Element column requested
	* \return Reference to matrix element
	*/
	inline const T& operator () (const int row, const int col) const {
		return m [col * 4 + row];
	}

	/**
	* \brief Returns element at index
	*
	* \param i index
	* \return Reference to matrix element at m[i]
	*/
	inline T& operator [] (int i) {
		return m[i];
	}

	/**
	* \brief Returns element at index
	*
	* \param i index
	* \return Reference to matrix element at m[i]
	*/
	inline const T& operator [] (const int i) const {
		return m[i];
	}

	/**
	* \brief Assign a matrix to another
	*
	* \param mat matrix to assign this matrix to
	*/
	inline void operator = (matrix4<T> mat) {

		for (int i=0; i<16; i++)
			m[i] = mat.m[i];
	}

	/**
	* \brief Assign a matrix to a scalar
	*
	* \param scalar Scalar value to set matrix elements to
	*/
	void operator = (T scalar) {

		m[0] = scalar;
		m[1] = scalar;
		m[2] = scalar;
		m[3] = scalar;
		m[4] = scalar;
		m[5] = scalar;
		m[6] = scalar;
		m[7] = scalar;
		m[8] = scalar;
		m[9] = scalar;
		m[10] = scalar;
		m[11] = scalar;
		m[12] = scalar;
		m[13] = scalar;
		m[14] = scalar;
		m[15] = scalar;
	}

	/**
	* \brief Test if two matrices are equal
	*
	* \param mat matrix to test with
	* \return true if matrix has same elements, false if not
	*/
	inline bool operator == (const matrix4<T>& mat) {

		for (int i= 0 ; i < 16; i++)
			if (m[i] != mat[i])
				return false;

		return true;
	}

	/**
	* \brief Test if two matrices are not equal
	*
	* \param mat matrix to test with
	* \return false if matrix has same elements, true if not
	*/
	inline bool operator != (const matrix4<T>& mat) {

		return !(*this == mat);
	}

	/**
	* \brief Add two matrices
	*
	* \param mat matrix to add
	* \return sum of this matrix added with mat
	*/
	matrix4<T> operator + (const matrix4<T>& mat) {

		matrix4<T> temp;

		temp.m[0] = m[0] + mat.m[0];
		temp.m[1] = m[1] + mat.m[1];
		temp.m[2] = m[2] + mat.m[2];
		temp.m[3] = m[3] + mat.m[3];
		temp.m[4] = m[4] + mat.m[4];
		temp.m[5] = m[5] + mat.m[5];
		temp.m[6] = m[6] + mat.m[6];
		temp.m[7] = m[7] + mat.m[7];
		temp.m[8] = m[8] + mat.m[8];
		temp.m[9] = m[9] + mat.m[9];
		temp.m[10] = m[10] + mat.m[10];
		temp.m[11] = m[11] + mat.m[11];
		temp.m[12] = m[12] + mat.m[12];
		temp.m[13] = m[13] + mat.m[13];
		temp.m[14] = m[14] + mat.m[14];
		temp.m[15] = m[15] + mat.m[15];

		return temp;
	}

	/**
	* \brief Subtract two matrices
	*
	* \param mat matrix to subtract
	* \return difference of this matrix subtracted with mat
	*/
	matrix4<T> operator - (const matrix4<T>& mat) {

		matrix4<T> temp;

		temp.m[0] = m[0] - mat.m[0];
		temp.m[1] = m[1] - mat.m[1];
		temp.m[2] = m[2] - mat.m[2];
		temp.m[3] = m[3] - mat.m[3];
		temp.m[4] = m[4] - mat.m[4];
		temp.m[5] = m[5] - mat.m[5];
		temp.m[6] = m[6] - mat.m[6];
		temp.m[7] = m[7] - mat.m[7];
		temp.m[8] = m[8] - mat.m[8];
		temp.m[9] = m[9] - mat.m[9];
		temp.m[10] = m[10] - mat.m[10];
		temp.m[11] = m[11] - mat.m[11];
		temp.m[12] = m[12] - mat.m[12];
		temp.m[13] = m[13] - mat.m[13];
		temp.m[14] = m[14] - mat.m[14];
		temp.m[15] = m[15] - mat.m[15];

		return temp;
	}

	/**
	* \brief Add and assign matrix
	*
	* \param mat matrix to add
	* \return this matrix
	*/
	matrix4<T> operator += (const matrix4<T>& mat) {

		m[0] += mat.m[0];
		m[1] += mat.m[1];
		m[2] += mat.m[2];
		m[3] += mat.m[3];
		m[4] += mat.m[4];
		m[5] += mat.m[5];
		m[6] += mat.m[6];
		m[7] += mat.m[7];
		m[8] += mat.m[8];
		m[9] += mat.m[9];
		m[10] += mat.m[10];
		m[11] += mat.m[11];
		m[12] += mat.m[12];
		m[13] += mat.m[13];
		m[14] += mat.m[14];
		m[15] += mat.m[15];
		return *this;
	}

	/**
	* \brief Subtract and assign matrix
	*
	* \param mat matrix to subtract
	* \return this matrix
	*/
	matrix4<T> operator -= (const matrix4<T>& mat) {

		m[0] -= mat.m[0];
		m[1] -= mat.m[1];
		m[2] -= mat.m[2];
		m[3] -= mat.m[3];
		m[4] -= mat.m[4];
		m[5] -= mat.m[5];
		m[6] -= mat.m[6];
		m[7] -= mat.m[7];
		m[8] -= mat.m[8];
		m[9] -= mat.m[9];
		m[10] -= mat.m[10];
		m[11] -= mat.m[11];
		m[12] -= mat.m[12];
		m[13] -= mat.m[13];
		m[14] -= mat.m[14];
		m[15] -= mat.m[15];
		return *this;
	}

	/**
	* \brief Multiply matrix by vector
	*
	* \param v vector
	* \return resulting matrix
	*/
	inline HgVector operator * (const HgVector b) {

		HgVector ret;

		ret.x = m11 * b.x + m12 * b.y + m13 * b.z + m14 * b.w;
		ret.y = m21 * b.x + m22 * b.y + m23 * b.z + m24 * b.w;;
		ret.z = m31 * b.x + m32 * b.y + m33 * b.z + m34 * b.w;;
		ret.w = m41 * b.x + m42 * b.y + m43 * b.z + m44 * b.w;;

		return ret;
	}

	/**
	* \brief Multiply two matrices together
	*
	* \param mat matrix to multiply by
	* \return resulting matrix
	*/
	matrix4<T> operator * (const matrix4<T>& mat) {

		matrix4<T> matrix;

		const T* m1 = m;
		const T* m2 = &mat.m[0];
		T* m3 = matrix.m;

		m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
		
		m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
		
		m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
		
		m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

		return matrix;
	}

	/**
	* \brief Multiply and assign two matrices together
	*
	* \param mat matrix to multiply by
	* \return resulting matrix
	*/
	matrix4<T> operator *= (const matrix4<T>& mat) {

		float matrix[16];
		const float* m1 = m;
		const float* m2 = mat.m;

		matrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		matrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		matrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		matrix[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
		
		matrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		matrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		matrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		matrix[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
		
		matrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		matrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		matrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		matrix[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
		
		matrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		matrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		matrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		matrix[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

		memcpy(m, matrix, 16 * sizeof( float ) );

		return *this;
	}

	/**
	* \brief Multiply by scalar
	*
	* \param scalar scalar to multiply by
	* \return resulting matrix
	*/
	inline matrix4<T> operator * (const T scalar) {

		matrix4<T> temp;

		temp.m[0] = m[0] * scalar;
		temp.m[1] = m[1] * scalar;
		temp.m[2] = m[2] * scalar;
		temp.m[3] = m[3] * scalar;
		temp.m[4] = m[4] * scalar;
		temp.m[5] = m[5] * scalar;
		temp.m[6] = m[6] * scalar;
		temp.m[7] = m[7] * scalar;
		temp.m[8] = m[8] * scalar;
		temp.m[9] = m[9] * scalar;
		temp.m[10] = m[10] * scalar;
		temp.m[11] = m[11] * scalar;
		temp.m[12] = m[12] * scalar;
		temp.m[13] = m[13] * scalar;
		temp.m[14] = m[14] * scalar;
		temp.m[15] = m[15] * scalar;

		return temp;
	}

	/**
	* \brief Multiply and assign by scalar
	*
	* \param scalar scalar to multiply by
	* \return this matrix
	*/
	inline matrix4<T> operator *= (const T scalar) {

		m[0] *= scalar;
		m[1] *= scalar;
		m[2] *= scalar;
		m[3] *= scalar;
		m[4] *= scalar;
		m[5] *= scalar;
		m[6] *= scalar;
		m[7] *= scalar;
		m[8] *= scalar;
		m[9] *= scalar;
		m[10] *= scalar;
		m[11] *= scalar;
		m[12] *= scalar;
		m[13] *= scalar;
		m[14] *= scalar;
		m[15] *= scalar;
		return *this;
	}

	/**
	* \brief Returns 3x3 sub matrix
	*
	*/
	matrix4<T> subMatrix (int i, int j) {

		matrix4<T> mat;
		int idst,jdst;

		for (int ti = 0; ti < 4; ti++)
			if (ti < i)
				idst = ti;
			else
			if (ti > i)
				idst = ti-1;

		for (int tj = 0; tj < 4; tj++)
			if (tj < j)
				jdst = tj;
			else
			if (tj > j)
				jdst = tj-1;

//        if (ti != i && tj != j)
    //		mat.m [ idst*3 + jdst*3 ] = m [ ti*4 + tj ];

		return mat;
	}

	/**
	* \brief Calculate determinant of matrix
	* \return determinant of matrix
	*/
	float determinant () {

		return
			m[12]*m[9]*m[6]*m[3]-
			m[8]*m[13]*m[6]*m[3]-
			m[12]*m[5]*m[10]*m[3]+
			m[4]*m[13]*m[10]*m[3]+
			m[8]*m[5]*m[14]*m[3]-
			m[4]*m[9]*m[14]*m[3]-
			m[12]*m[9]*m[2]*m[7]+
			m[8]*m[13]*m[2]*m[7]+
			m[12]*m[1]*m[10]*m[7]-
			m[0]*m[13]*m[10]*m[7]-
			m[8]*m[1]*m[14]*m[7]+
			m[0]*m[9]*m[14]*m[7]+
			m[12]*m[5]*m[2]*m[11]-
			m[4]*m[13]*m[2]*m[11]-
			m[12]*m[1]*m[6]*m[11]+
			m[0]*m[13]*m[6]*m[11]+
			m[4]*m[1]*m[14]*m[11]-
			m[0]*m[5]*m[14]*m[11]-
			m[8]*m[5]*m[2]*m[15]+
			m[4]*m[9]*m[2]*m[15]+
			m[8]*m[1]*m[6]*m[15]-
			m[0]*m[9]*m[6]*m[15]-
			m[4]*m[1]*m[10]*m[15]+
			m[0]*m[5]*m[10]*m[15];
	}

	/**
	* \brief Calculate inverse of matrix
	* \param i matrix
	* \return true if successful, false if not
	*/
	bool inverse (matrix4<float>& i) {

		float x = determinant ();
		if (x == 0.0f)
			return false;

		i[0]= (-m[13]*m[10]*m[7] +m[9]*m[14]*m[7] +m[13]*m[6]*m[11]
			-m[5]*m[14]*m[11] -m[9]*m[6]*m[15] +m[5]*m[10]*m[15])/x;
		i[4]= ( m[12]*m[10]*m[7] -m[8]*m[14]*m[7] -m[12]*m[6]*m[11]
			+m[4]*m[14]*m[11] +m[8]*m[6]*m[15] -m[4]*m[10]*m[15])/x;
		i[8]= (-m[12]*m[9]* m[7] +m[8]*m[13]*m[7] +m[12]*m[5]*m[11]
			-m[4]*m[13]*m[11] -m[8]*m[5]*m[15] +m[4]*m[9]* m[15])/x;
		i[12]=( m[12]*m[9]* m[6] -m[8]*m[13]*m[6] -m[12]*m[5]*m[10]
			+m[4]*m[13]*m[10] +m[8]*m[5]*m[14] -m[4]*m[9]* m[14])/x;
		i[1]= ( m[13]*m[10]*m[3] -m[9]*m[14]*m[3] -m[13]*m[2]*m[11]
			+m[1]*m[14]*m[11] +m[9]*m[2]*m[15] -m[1]*m[10]*m[15])/x;
		i[5]= (-m[12]*m[10]*m[3] +m[8]*m[14]*m[3] +m[12]*m[2]*m[11]
			-m[0]*m[14]*m[11] -m[8]*m[2]*m[15] +m[0]*m[10]*m[15])/x;
		i[9]= ( m[12]*m[9]* m[3] -m[8]*m[13]*m[3] -m[12]*m[1]*m[11]
			+m[0]*m[13]*m[11] +m[8]*m[1]*m[15] -m[0]*m[9]* m[15])/x;
		i[13]=(-m[12]*m[9]* m[2] +m[8]*m[13]*m[2] +m[12]*m[1]*m[10]
			-m[0]*m[13]*m[10] -m[8]*m[1]*m[14] +m[0]*m[9]* m[14])/x;
		i[2]= (-m[13]*m[6]* m[3] +m[5]*m[14]*m[3] +m[13]*m[2]*m[7]
			-m[1]*m[14]*m[7] -m[5]*m[2]*m[15] +m[1]*m[6]* m[15])/x;
		i[6]= ( m[12]*m[6]* m[3] -m[4]*m[14]*m[3] -m[12]*m[2]*m[7]
			+m[0]*m[14]*m[7] +m[4]*m[2]*m[15] -m[0]*m[6]* m[15])/x;
		i[10]=(-m[12]*m[5]* m[3] +m[4]*m[13]*m[3] +m[12]*m[1]*m[7]
			-m[0]*m[13]*m[7] -m[4]*m[1]*m[15] +m[0]*m[5]* m[15])/x;
		i[14]=( m[12]*m[5]* m[2] -m[4]*m[13]*m[2] -m[12]*m[1]*m[6]
			+m[0]*m[13]*m[6] +m[4]*m[1]*m[14] -m[0]*m[5]* m[14])/x;
		i[3]= ( m[9]* m[6]* m[3] -m[5]*m[10]*m[3] -m[9]* m[2]*m[7]
			+m[1]*m[10]*m[7] +m[5]*m[2]*m[11] -m[1]*m[6]* m[11])/x;
		i[7]= (-m[8]* m[6]* m[3] +m[4]*m[10]*m[3] +m[8]* m[2]*m[7]
			-m[0]*m[10]*m[7] -m[4]*m[2]*m[11] +m[0]*m[6]* m[11])/x;
		i[11]=( m[8]* m[5]* m[3] -m[4]*m[9]* m[3] -m[8]* m[1]*m[7]
			+m[0]*m[9]* m[7] +m[4]*m[1]*m[11] -m[0]*m[5]* m[11])/x;
		i[15]=(-m[8]* m[5]* m[2] +m[4]*m[9]* m[2] +m[8]* m[1]*m[6]
			-m[0]*m[9]* m[6] -m[4]*m[1]*m[10] +m[0]*m[5]* m[10])/x;

		return true;
	}

	void transpose () {

		// temporary storage
		matrix4<float> ret;

		ret.m11 = m11;
		ret.m12 = m21;
		ret.m13 = m31;
		ret.m14 = m41;

		ret.m21 = m12;
		ret.m22 = m22;
		ret.m23 = m32;
		ret.m24 = m42;

		ret.m31 = m13;
		ret.m32 = m23;
		ret.m33 = m33;
		ret.m34 = m43;

		ret.m41 = m14;
		ret.m42 = m24;
		ret.m43 = m34;
		ret.m44 = m44;

		for (int i=0; i<16; i++)
			m[i] = ret[i];
	}
};

typedef matrix4<float> HgMatrix;

} // namespace hgm

#endif
