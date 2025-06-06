/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* $Header: /Commando/Code/wwmath/matrix3.h 36    5/11/01 7:11p Jani_p $ */
/*********************************************************************************************** 
 ***                            Confidential - Westwood Studios                              *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : WW3D                                                         * 
 *                                                                                             * 
 *                    File Name : MATRIX3.H                                                    * 
 *                                                                                             * 
#ifdef OG
 *                   Programmer : Greg Hjelstrom                                               * 

#endif
#ifdef ZH
 *               Org Programmer : Greg Hjelstrom                                               * 
 *                                                                                             * 
 *                       Author : Kenny Mitchell                                               * 
#endif
 *                                                                                             * 
 *                   Start Date : 06/02/97                                                     * 
 *                                                                                             * 
#ifdef OG
 *                  Last Update : June 2, 1997 [GH]                                            * 
#endif
#ifdef ZH
 *                  Last Update : June 6, 2002 [KM]                                            * 
#endif
 *                                                                                             * 
#ifdef ZH
 * 06/26/02 KM Matrix name change to avoid MAX conflicts                                       *
#endif
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
#ifdef OG
 *   Matrix3::Matrix3 -- Constructor, optionally initialize to Identitiy matrix                * 
 *   Matrix3::Matrix3 -- Copy Constructor                                                      * 
 *   Matrix3::Matrix3 -- Convert a Matrix3D (fake 4x4) to a Matrix3                            * 
 *   Matrix3::Matrix3 -- Constructor                                                           * 
 *   Matrix3::Transpose -- Returns transpose of the matrix                                     * 
 *   Matrix3::Inverse -- returns the inverse of the matrix                                     * 
 *   Matrix3::Determinant -- returns the determinant of the matrix                             *
 *   Matrix3::operator = -- assignment operator                                                * 
 *   Matrix3::operator += -- "plus equals" operator                                            * 
 *   Matrix3::operator-= -- "minus equals" operator                                            * 
 *   Matrix3::operator *= -- "times equals" operator                                           * 
 *   Matrix3::operator /= -- "divide equals" operator                                          * 
#endif
#ifdef ZH
 *   Matrix3x3::Matrix3x3 -- Constructor, optionally initialize to Identitiy matrix                * 
 *   Matrix3x3::Matrix3x3 -- Copy Constructor                                                      * 
 *   Matrix3x3::Matrix3x3 -- Convert a Matrix3D (fake 4x4) to a Matrix3x3                            * 
 *   Matrix3x3::Matrix3x3 -- Constructor                                                           * 
 *   Matrix3x3::Transpose -- Returns transpose of the matrix                                     * 
 *   Matrix3x3::Inverse -- returns the inverse of the matrix                                     * 
 *   Matrix3x3::Determinant -- returns the determinant of the matrix                             *
 *   Matrix3x3::operator = -- assignment operator                                                * 
 *   Matrix3x3::operator += -- "plus equals" operator                                            * 
 *   Matrix3x3::operator-= -- "minus equals" operator                                            * 
 *   Matrix3x3::operator *= -- "times equals" operator                                           * 
 *   Matrix3x3::operator /= -- "divide equals" operator                                          * 
#endif
 *   Create_X_Rotation_Matrix3 -- creates a matrix3 which is a rotation about X                * 
#ifdef OG
 *   Create_Y_Rotation_Matrix3 -- Creates a Matrix3 which is a rotation about Y                * 
#endif
#ifdef ZH
 *   Create_Y_Rotation_Matrix3 -- Creates a Matrix3x3 which is a rotation about Y                * 
#endif
 *   Create_Z_Rotation_Matrix3 -- Creates a matrix3 which is a rotation about Z                * 
#ifdef OG
 *   Matrix3::Rotate_X -- Post-mutiplies an x rotation onto the current matrix                 * 
 *   Matrix3::Rotate_Y -- Post-multiplies the matrix with a rotation about Y                   * 
 *   Matrix3::Rotate_Z -- Post-multiplies the matrix with a rotation about Z                   * 
#endif
#ifdef ZH
 *   Matrix3x3::Rotate_X -- Post-mutiplies an x rotation onto the current matrix                 * 
 *   Matrix3x3::Rotate_Y -- Post-multiplies the matrix with a rotation about Y                   * 
 *   Matrix3x3::Rotate_Z -- Post-multiplies the matrix with a rotation about Z                   * 
#endif
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#if defined(_MSC_VER)
#pragma once
#endif

#ifndef Matrix3_H
#define Matrix3_H

#include "always.h"
#include "vector3.h"
#include "wwdebug.h"
#ifdef _UNIX
#include "osdep.h"
#endif


class Matrix3D;
#ifdef OG
class Matrix4;
#endif
#ifdef ZH
class Matrix4x4;
#endif
class Quaternion;

#ifdef OG
class Matrix3
#endif
#ifdef ZH
class Matrix3x3
#endif
{
public:

	/*
	** Constructors
	*/
#ifdef OG
	WWINLINE Matrix3(void) {};
	WWINLINE Matrix3(const Matrix3 & m);
#endif
#ifdef ZH
	WWINLINE Matrix3x3(void) {};
	WWINLINE Matrix3x3(const Matrix3x3 & m);
#endif

#ifdef OG
	WWINLINE explicit Matrix3(bool identity);
	WWINLINE explicit Matrix3(const Vector3 & v0, const Vector3 & v1, const Vector3 & v2);
	explicit Matrix3(const Matrix3D & m);
	explicit Matrix3(const Matrix4 & m);
	WWINLINE explicit Matrix3(
#endif
#ifdef ZH
	WWINLINE explicit Matrix3x3(bool identity);
	WWINLINE explicit Matrix3x3(const Vector3 & v0, const Vector3 & v1, const Vector3 & v2);
	explicit Matrix3x3(const Matrix3D & m);
	explicit Matrix3x3(const Matrix4x4 & m);
	WWINLINE explicit Matrix3x3(
#endif
		float m11,float m12,float m13,
		float m21,float m22,float m23,
		float m31,float m32,float m33
	);
#ifdef OG
	WWINLINE explicit Matrix3(const Vector3 & axis,float angle);
	WWINLINE explicit Matrix3(const Vector3 & axis,float s_angle,float c_angle);
	WWINLINE explicit Matrix3(const Quaternion & q);
#endif
#ifdef ZH
	WWINLINE explicit Matrix3x3(const Vector3 & axis,float angle);
	WWINLINE explicit Matrix3x3(const Vector3 & axis,float s_angle,float c_angle);
	WWINLINE explicit Matrix3x3(const Quaternion & q);
#endif

	/*
	** 'Set' functions
	*/
	void Set(const Matrix3D & m);
#ifdef OG
	void Set(const Matrix4 & m);
#endif
#ifdef ZH
	void Set(const Matrix4x4 & m);
#endif
	WWINLINE void Set(const Vector3 & v0, const Vector3 & v1, const Vector3 & v2);
	WWINLINE void Set(
		float m11,float m12,float m13,
		float m21,float m22,float m23,
		float m31,float m32,float m33
	);
	WWINLINE void Set(const Vector3 & axis,float angle);
	WWINLINE void Set(const Vector3 & axis,float s_angle,float c_angle);
	void Set(const Quaternion & q);
	
	/*
	** Access operators
	*/
	WWINLINE Vector3 & operator [] (int i) { return Row[i]; }
	WWINLINE const Vector3 & operator [] (int i) const { return Row[i]; }

	/*
	** Transpose and Inverse
	*/
#ifdef OG
	WWINLINE Matrix3	Transpose	(void) const;
	WWINLINE Matrix3	Inverse		(void) const;
#endif
#ifdef ZH
	WWINLINE Matrix3x3	Transpose	(void) const;
	WWINLINE Matrix3x3	Inverse		(void) const;
#endif
	WWINLINE float		Determinant	(void) const;

	/*
	** Assignment operators
	*/
#ifdef OG
	WWINLINE Matrix3 & operator = (const Matrix3 & m);
	Matrix3 & operator = (const Matrix3D & m);
	Matrix3 & operator = (const Matrix4 & m);
#endif
#ifdef ZH
	WWINLINE Matrix3x3 & operator = (const Matrix3x3 & m);
	Matrix3x3 & operator = (const Matrix3D & m);
	Matrix3x3 & operator = (const Matrix4x4 & m);
#endif

#ifdef OG
	WWINLINE Matrix3 & operator += (const Matrix3 & m);
	WWINLINE Matrix3 & operator -= (const Matrix3 & m);
	WWINLINE Matrix3 & operator *= (float d);
	WWINLINE Matrix3 & operator /= (float d);
#endif
#ifdef ZH
	WWINLINE Matrix3x3 & operator += (const Matrix3x3 & m);
	WWINLINE Matrix3x3 & operator -= (const Matrix3x3 & m);
	WWINLINE Matrix3x3 & operator *= (float d);
	WWINLINE Matrix3x3 & operator /= (float d);
#endif

	WWINLINE void	Make_Identity(void);

	/*
	** Automatically concatenate a rotation onto the current matrix
	*/
	WWINLINE void Rotate_X(float theta);
	WWINLINE void Rotate_X(float s,float c);

	WWINLINE void Rotate_Y(float theta);
	WWINLINE void Rotate_Y(float s,float c);

	WWINLINE void Rotate_Z(float theta);
	WWINLINE void Rotate_Z(float s,float c);

	/*
	** These functions will give you the approximate amount that the
	** matrix has been rotated about a given axis.  These functions
	** cannot be used to re-build a matrx.  Use the EulerAnglesClass
	** to convert a matrix into a set of three Euler angles.
	*/
	float Get_X_Rotation(void) const;
	float Get_Y_Rotation(void) const;
	float Get_Z_Rotation(void) const;
	
	/*
	** These functions return a vector representing the direction an
   ** axis is pointing.
   */
	WWINLINE Vector3 Get_X_Vector(void) const;
	WWINLINE Vector3 Get_Y_Vector(void) const;
	WWINLINE Vector3 Get_Z_Vector(void) const;
	WWINLINE void Get_X_Vector(Vector3 * set_x) const;
	WWINLINE void Get_Y_Vector(Vector3 * set_y) const;
	WWINLINE void Get_Z_Vector(Vector3 * set_z) const;
	
	/*
	** Negation
	*/
#ifdef OG
	WWINLINE friend Matrix3 operator - (const Matrix3& a);
#endif
#ifdef ZH
	WWINLINE friend Matrix3x3 operator - (const Matrix3x3& a);
#endif
	
	/*
	** Scalar multiplication and division
	*/
#ifdef OG
	WWINLINE friend Matrix3 operator * (const Matrix3& a,float d);
	WWINLINE friend Matrix3 operator * (float d,const Matrix3& a);
	WWINLINE friend Matrix3 operator / (const Matrix3& a,float d);
#endif
#ifdef ZH
	WWINLINE friend Matrix3x3 operator * (const Matrix3x3& a,float d);
	WWINLINE friend Matrix3x3 operator * (float d,const Matrix3x3& a);
	WWINLINE friend Matrix3x3 operator / (const Matrix3x3& a,float d);
#endif

	/*
	** matrix addition
	*/ 
#ifdef OG
	WWINLINE friend Matrix3 operator + (const Matrix3& a, const Matrix3& b);
	static WWINLINE void Add(const Matrix3 & a, const Matrix3 & b,Matrix3 * res);
#endif
#ifdef ZH
	WWINLINE friend Matrix3x3 operator + (const Matrix3x3& a, const Matrix3x3& b);
	static WWINLINE void Add(const Matrix3x3 & a, const Matrix3x3 & b,Matrix3x3 * res);
#endif

	/*
	** matrix subtraction
	*/
#ifdef OG
	WWINLINE friend Matrix3 operator - (const Matrix3 & a, const Matrix3 & b);
	static WWINLINE void Subtract(const Matrix3 & a, const Matrix3 & b,Matrix3 * res);
#endif
#ifdef ZH
	WWINLINE friend Matrix3x3 operator - (const Matrix3x3 & a, const Matrix3x3 & b);
	static WWINLINE void Subtract(const Matrix3x3 & a, const Matrix3x3 & b,Matrix3x3 * res);
#endif

	/*
	** matrix multiplication
	*/
#ifdef OG
	WWINLINE friend Matrix3 operator * (const Matrix3 & a, const Matrix3 & b);
	WWINLINE friend Matrix3 operator * (const Matrix3D & a, const Matrix3 & b);
	WWINLINE friend Matrix3 operator * (const Matrix3 & a, const Matrix3D & b);
#endif
#ifdef ZH
	WWINLINE friend Matrix3x3 operator * (const Matrix3x3 & a, const Matrix3x3 & b);
	WWINLINE friend Matrix3x3 operator * (const Matrix3D & a, const Matrix3x3 & b);
	WWINLINE friend Matrix3x3 operator * (const Matrix3x3 & a, const Matrix3D & b);
#endif

#ifdef OG
	static void Multiply(const Matrix3 & a, const Matrix3 & b,Matrix3 * res);
	static void Multiply(const Matrix3D & a, const Matrix3 & b,Matrix3 * res);
	static void Multiply(const Matrix3 & a, const Matrix3D & b,Matrix3 * res);
#endif
#ifdef ZH
	static void Multiply(const Matrix3x3 & a, const Matrix3x3 & b,Matrix3x3 * res);
	static void Multiply(const Matrix3D & a, const Matrix3x3 & b,Matrix3x3 * res);
	static void Multiply(const Matrix3x3 & a, const Matrix3D & b,Matrix3x3 * res);
#endif

	/*
	** Matrix-Vector multiplication
	*/
#ifdef OG
	WWINLINE friend Vector3 operator * (const Matrix3 & a, const Vector3 & v);
	static WWINLINE void	Rotate_Vector(const Matrix3 & tm,const Vector3 & in,Vector3 * out);
	static WWINLINE void	Transpose_Rotate_Vector(const Matrix3 & tm,const Vector3 & in,Vector3 * out);
#endif
#ifdef ZH
	WWINLINE friend Vector3 operator * (const Matrix3x3 & a, const Vector3 & v);
	static WWINLINE void	Rotate_Vector(const Matrix3x3 & tm,const Vector3 & in,Vector3 * out);
	static WWINLINE void	Transpose_Rotate_Vector(const Matrix3x3 & tm,const Vector3 & in,Vector3 * out);
#endif

	/*
	** Comparison operators
	*/
#ifdef OG
	WWINLINE friend int operator == (const Matrix3 & a, const Matrix3 & b);
	WWINLINE friend int operator != (const Matrix3 & a, const Matrix3 & b);
#endif
#ifdef ZH
	WWINLINE friend int operator == (const Matrix3x3 & a, const Matrix3x3 & b);
	WWINLINE friend int operator != (const Matrix3x3 & a, const Matrix3x3 & b);
#endif

	/*
	** Swap two matrices in place
	*/
#ifdef OG
	WWINLINE friend void Swap(Matrix3 & a,Matrix3 & b);
#endif
#ifdef ZH
	WWINLINE friend void Swap(Matrix3x3 & a,Matrix3x3 & b);
#endif

	/*
	** Check whether a matrix is orthogonal, make it orthogonal
	*/
	int	Is_Orthogonal(void) const; 
	void	Re_Orthogonalize(void);

	/*
	** Miscellaneous
	*/
	WWINLINE void	Rotate_AABox_Extent(const Vector3 & extent,Vector3 * new_extent);

	/*
#ifdef OG
	** Some useful pre-initialized Matrix3's
#endif
#ifdef ZH
	** Some useful pre-initialized Matrix3x3's
#endif
	*/
#ifdef OG
	static const Matrix3			Identity;
	static const Matrix3			RotateX90;
	static const Matrix3			RotateX180;
	static const Matrix3			RotateX270;
	static const Matrix3			RotateY90;
	static const Matrix3			RotateY180;
	static const Matrix3			RotateY270;
	static const Matrix3			RotateZ90;
	static const Matrix3			RotateZ180;
	static const Matrix3			RotateZ270;
#endif
#ifdef ZH
	static const Matrix3x3			Identity;
	static const Matrix3x3			RotateX90;
	static const Matrix3x3			RotateX180;
	static const Matrix3x3			RotateX270;
	static const Matrix3x3			RotateY90;
	static const Matrix3x3			RotateY180;
	static const Matrix3x3			RotateY270;
	static const Matrix3x3			RotateZ90;
	static const Matrix3x3			RotateZ180;
	static const Matrix3x3			RotateZ270;
#endif

protected:

	Vector3 Row[3];

};


/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Matrix3 -- Constructor, optionally initialize to Identitiy matrix                  * 
#endif
#ifdef ZH
 * Matrix3x3::Matrix3x3 -- Constructor, optionally initialize to Identitiy matrix                  * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/02/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3::Matrix3(bool identity)
#endif
#ifdef ZH
WWINLINE Matrix3x3::Matrix3x3(bool identity)
#endif
{
	if (identity) {
		Row[0].Set(1.0,0.0,0.0);
		Row[1].Set(0.0,1.0,0.0);
		Row[2].Set(0.0,0.0,1.0);
	}
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Matrix3 -- Copy Constructor                                                        * 
#endif
#ifdef ZH
 * Matrix3x3::Matrix3x3 -- Copy Constructor                                                        * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/02/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3::Matrix3(const Matrix3 & m)
#endif
#ifdef ZH
WWINLINE Matrix3x3::Matrix3x3(const Matrix3x3 & m)
#endif
{
	Row[0] = m.Row[0]; Row[1] = m.Row[1]; Row[2] = m.Row[2];
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Matrix3 -- Constructor                                                             * 
#endif
#ifdef ZH
 * Matrix3x3::Matrix3x3 -- Constructor                                                             * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/02/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3::Matrix3(const Vector3 & r0, const Vector3 & r1, const Vector3 & r2)
#endif
#ifdef ZH
WWINLINE Matrix3x3::Matrix3x3(const Vector3 & r0, const Vector3 & r1, const Vector3 & r2)
#endif
{ 
	Row[0] = r0; 
	Row[1] = r1; 
	Row[2] = r2; 
}

#ifdef OG
WWINLINE void Matrix3::Set(const Vector3 & r0, const Vector3 & r1, const Vector3 & r2)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Set(const Vector3 & r0, const Vector3 & r1, const Vector3 & r2)
#endif
{ 
	Row[0] = r0; 
	Row[1] = r1; 
	Row[2] = r2; 
}

#ifdef OG
WWINLINE void Matrix3::Make_Identity(void)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Make_Identity(void)
#endif
{
	Row[0].Set(1.0f,0.0f,0.0f);
	Row[1].Set(0.0f,1.0f,0.0f);
	Row[2].Set(0.0f,0.0f,1.0f);
}


#ifdef OG
WWINLINE Matrix3::Matrix3
#endif
#ifdef ZH
WWINLINE Matrix3x3::Matrix3x3
#endif
(
	float m11,float m12,float m13,
	float m21,float m22,float m23,
	float m31,float m32,float m33
)
{
	Row[0].Set(m11,m12,m13);
	Row[1].Set(m21,m22,m23);
	Row[2].Set(m31,m32,m33);
}

#ifdef OG
WWINLINE void Matrix3::Set
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Set
#endif
(
	float m11,float m12,float m13,
	float m21,float m22,float m23,
	float m31,float m32,float m33
)
{
	Row[0].Set(m11,m12,m13);
	Row[1].Set(m21,m22,m23);
	Row[2].Set(m31,m32,m33);
}

#ifdef OG
WWINLINE Matrix3::Matrix3(const Vector3 & axis,float angle) 
#endif
#ifdef ZH
WWINLINE Matrix3x3::Matrix3x3(const Vector3 & axis,float angle) 
#endif
{ 
	Set(axis,angle); 
}

#ifdef OG
WWINLINE Matrix3::Matrix3(const Vector3 & axis,float s_angle,float c_angle) 
#endif
#ifdef ZH
WWINLINE Matrix3x3::Matrix3x3(const Vector3 & axis,float s_angle,float c_angle) 
#endif
{ 
	Set(axis,s_angle,c_angle); 
}

#ifdef OG
WWINLINE void Matrix3::Set(const Vector3 & axis,float angle)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Set(const Vector3 & axis,float angle)
#endif
{
	Set(axis,sinf(angle),cosf(angle));
}

#ifdef OG
WWINLINE void Matrix3::Set(const Vector3 & axis,float s,float c)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Set(const Vector3 & axis,float s,float c)
#endif
{
	WWASSERT(WWMath::Fabs(axis.Length2() - 1.0f) < 0.001f);

	Row[0].Set(
		(float)(axis[0]*axis[0] + c*(1.0f - axis[0]*axis[0])),
		(float)(axis[0]*axis[1]*(1.0f - c) - axis[2]*s),
		(float)(axis[2]*axis[0]*(1.0f - c) + axis[1]*s)
	);

	Row[1].Set(
		(float)(axis[0]*axis[1]*(1.0f - c) + axis[2]*s),
		(float)(axis[1]*axis[1] + c*(1.0f - axis[1]*axis[1])),
		(float)(axis[1]*axis[2]*(1.0f - c) - axis[0]*s)
	);

	Row[2].Set(
		(float)(axis[2]*axis[0]*(1.0f - c) - axis[1]*s),
		(float)(axis[1]*axis[2]*(1.0f - c) + axis[0]*s),
		(float)(axis[2]*axis[2] + c*(1 - axis[2]*axis[2]))
	);
}

#ifdef OG
WWINLINE Matrix3::Matrix3(const Quaternion & q)
#endif
#ifdef ZH
WWINLINE Matrix3x3::Matrix3x3(const Quaternion & q)
#endif
{
	this->Set(q);
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Transpose -- Returns transpose of the matrix                                       * 
#endif
#ifdef ZH
 * Matrix3x3::Transpose -- Returns transpose of the matrix                                       * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/02/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3 Matrix3::Transpose() const
#endif
#ifdef ZH
WWINLINE Matrix3x3 Matrix3x3::Transpose() const
#endif
{
#ifdef OG
    return Matrix3(
#endif
#ifdef ZH
    return Matrix3x3(
#endif
			Vector3(Row[0][0], Row[1][0], Row[2][0]),
			Vector3(Row[0][1], Row[1][1], Row[2][1]),
			Vector3(Row[0][2], Row[1][2], Row[2][2])
	);
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Inverse -- returns the inverse of the matrix                                       * 
#endif
#ifdef ZH
 * Matrix3x3::Inverse -- returns the inverse of the matrix                                       * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/02/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3 Matrix3::Inverse() const    // Gauss-Jordan elimination with partial pivoting
#endif
#ifdef ZH
WWINLINE Matrix3x3 Matrix3x3::Inverse() const    // Gauss-Jordan elimination with partial pivoting
#endif
{
#ifdef OG
	Matrix3 a(*this);				// As a evolves from original mat into identity
	Matrix3 b(true);				// b evolves from identity into inverse(a)
#endif
#ifdef ZH
	Matrix3x3 a(*this);				// As a evolves from original mat into identity
	Matrix3x3 b(true);				// b evolves from identity into inverse(a)
#endif
	int i, j, i1;

	// Loop over cols of a from left to right, eliminating above and below diagonal
	for (j=0; j<3; j++) {

		// Find largest pivot in column j among rows j..3
		i1 = j;
		for (i=j+1; i<3; i++) {
			if (WWMath::Fabs(a[i][j]) > WWMath::Fabs(a[i1][j])) {
				i1 = i;
			}
		}

		// Swap rows i1 and j in a and b to put pivot on diagonal
		Swap(a.Row[i1], a.Row[j]);
		Swap(b.Row[i1], b.Row[j]);

		// Scale row j to have a unit diagonal
		if (a[j][j]==0.) {
#ifdef OG
			//Matrix3::inverse: singular matrix; can't invert
#endif
#ifdef ZH
			//Matrix3x3::inverse: singular matrix; can't invert
#endif
		}
		b.Row[j] /= a.Row[j][j];
		a.Row[j] /= a.Row[j][j];

		// Eliminate off-diagonal elems in col j of a, doing identical ops to b
		for (i=0; i<3; i++) {
			if (i != j) {
				b.Row[i] -= a[i][j] * b.Row[j];
				a.Row[i] -= a[i][j] * a.Row[j];
			}
		}
	}
	return b;
}


/***********************************************************************************************
#ifdef OG
 * Matrix3::Determinant -- returns the determinant of the matrix                               *
#endif
#ifdef ZH
 * Matrix3x3::Determinant -- returns the determinant of the matrix                               *
#endif
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   1/7/20     DRM : Created.                                                                 *
 *=============================================================================================*/
#ifdef OG
WWINLINE float Matrix3::Determinant(void) const 
#endif
#ifdef ZH
WWINLINE float Matrix3x3::Determinant(void) const 
#endif
{
	return   Row[0][0] * (Row[1][1] * Row[2][2] - Row[1][2] * Row[2][1])
		    - Row[0][1] * (Row[1][0] * Row[2][2] - Row[1][2] * Row[2][0])
			 - Row[0][2] * (Row[1][0] * Row[2][1] - Row[1][1] * Row[2][0]);
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::operator = -- assignment operator                                                  * 
#endif
#ifdef ZH
 * Matrix3x3::operator = -- assignment operator                                                  * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/02/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3 & Matrix3::operator = (const Matrix3 & m)
#endif
#ifdef ZH
WWINLINE Matrix3x3 & Matrix3x3::operator = (const Matrix3x3 & m)
#endif
{
	Row[0] = m.Row[0]; Row[1] = m.Row[1]; Row[2] = m.Row[2];
	return *this; 
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::operator += -- "plus equals" operator                                              * 
#endif
#ifdef ZH
 * Matrix3x3::operator += -- "plus equals" operator                                              * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/02/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3& Matrix3::operator += (const Matrix3 & m)
#endif
#ifdef ZH
WWINLINE Matrix3x3& Matrix3x3::operator += (const Matrix3x3 & m)
#endif
{
	Row[0] += m.Row[0]; Row[1] += m.Row[1]; Row[2] += m.Row[2]; 
	return *this; 
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::operator-= -- "minus equals" operator                                              * 
#endif
#ifdef ZH
 * Matrix3x3::operator-= -- "minus equals" operator                                              * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/02/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3& Matrix3::operator -= (const Matrix3 & m)
#endif
#ifdef ZH
WWINLINE Matrix3x3& Matrix3x3::operator -= (const Matrix3x3 & m)
#endif
{
	Row[0] -= m.Row[0]; Row[1] -= m.Row[1]; Row[2] -= m.Row[2]; 
	return *this; 
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::operator *= -- "times equals" operator                                             * 
#endif
#ifdef ZH
 * Matrix3x3::operator *= -- "times equals" operator                                             * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/02/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3& Matrix3::operator *= (float d)
#endif
#ifdef ZH
WWINLINE Matrix3x3& Matrix3x3::operator *= (float d)
#endif
{
	Row[0] *= d; Row[1] *= d; Row[2] *= d; 
	return *this; 
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::operator /= -- "divide equals" operator                                            * 
#endif
#ifdef ZH
 * Matrix3x3::operator /= -- "divide equals" operator                                            * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   06/02/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3& Matrix3::operator /= (float d)
#endif
#ifdef ZH
WWINLINE Matrix3x3& Matrix3x3::operator /= (float d)
#endif
{
	Row[0] /= d; Row[1] /= d; Row[2] /= d; 
	return *this; 
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Get_X_Rotation -- approximates the rotation about the X axis                       * 
#endif
#ifdef ZH
 * Matrix3x3::Get_X_Rotation -- approximates the rotation about the X axis                       * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   08/11/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE float Matrix3::Get_X_Rotation(void) const
#endif
#ifdef ZH
WWINLINE float Matrix3x3::Get_X_Rotation(void) const
#endif
{
	Vector3 v = (*this) * Vector3(0.0,1.0,0.0);
	return WWMath::Atan2(v[2], v[1]);
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Get_Y_Rotation -- approximates the rotation about the Y axis                       * 
#endif
#ifdef ZH
 * Matrix3x3::Get_Y_Rotation -- approximates the rotation about the Y axis                       * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   08/11/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE float Matrix3::Get_Y_Rotation(void) const 
#endif
#ifdef ZH
WWINLINE float Matrix3x3::Get_Y_Rotation(void) const 
#endif
{
	Vector3 v = (*this) * Vector3(0.0,0.0,1.0);
	return WWMath::Atan2(v[0],v[2]);
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Get_Z_Rotation -- approximates the rotation about the Z axis                       * 
#endif
#ifdef ZH
 * Matrix3x3::Get_Z_Rotation -- approximates the rotation about the Z axis                       * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   08/11/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE float Matrix3::Get_Z_Rotation(void) const
#endif
#ifdef ZH
WWINLINE float Matrix3x3::Get_Z_Rotation(void) const
#endif
{
	Vector3 v = (*this) * Vector3(1.0,0.0,0.0);
	return WWMath::Atan2(v[1],v[0]);
}

#ifdef OG
WWINLINE Vector3 Matrix3::Get_X_Vector(void) const
#endif
#ifdef ZH
WWINLINE Vector3 Matrix3x3::Get_X_Vector(void) const
#endif
{
   return Vector3(Row[0][0], Row[1][0], Row[2][0]);
}

#ifdef OG
WWINLINE Vector3 Matrix3::Get_Y_Vector(void) const
#endif
#ifdef ZH
WWINLINE Vector3 Matrix3x3::Get_Y_Vector(void) const
#endif
{
   return Vector3(Row[0][1], Row[1][1], Row[2][1]);
}

#ifdef OG
WWINLINE Vector3 Matrix3::Get_Z_Vector(void) const
#endif
#ifdef ZH
WWINLINE Vector3 Matrix3x3::Get_Z_Vector(void) const
#endif
{
   return Vector3(Row[0][2], Row[1][2], Row[2][2]);
}

#ifdef OG
WWINLINE void Matrix3::Get_X_Vector(Vector3 * set) const
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Get_X_Vector(Vector3 * set) const
#endif
{
   set->Set(Row[0][0], Row[1][0], Row[2][0]);
}

#ifdef OG
WWINLINE void Matrix3::Get_Y_Vector(Vector3 * set) const
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Get_Y_Vector(Vector3 * set) const
#endif
{
   set->Set(Row[0][1], Row[1][1], Row[2][1]);
}

#ifdef OG
WWINLINE void Matrix3::Get_Z_Vector(Vector3 * set) const
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Get_Z_Vector(Vector3 * set) const
#endif
{
   set->Set(Row[0][2], Row[1][2], Row[2][2]);
}

#ifdef OG
WWINLINE Matrix3 operator - (const Matrix3 & a)
#endif
#ifdef ZH
WWINLINE Matrix3x3 operator - (const Matrix3x3 & a)
#endif
{ 
#ifdef OG
	return Matrix3(-a.Row[0], -a.Row[1], -a.Row[2]); 
#endif
#ifdef ZH
	return Matrix3x3(-a.Row[0], -a.Row[1], -a.Row[2]); 
#endif
}

#ifdef OG
WWINLINE Matrix3 operator * (const Matrix3 & a, float d)
#endif
#ifdef ZH
WWINLINE Matrix3x3 operator * (const Matrix3x3 & a, float d)
#endif
{ 
#ifdef OG
	return Matrix3(a.Row[0] * d, a.Row[1] * d, a.Row[2] * d); 
#endif
#ifdef ZH
	return Matrix3x3(a.Row[0] * d, a.Row[1] * d, a.Row[2] * d); 
#endif
}

#ifdef OG
WWINLINE Matrix3 operator * (float d, const Matrix3 & a)
#endif
#ifdef ZH
WWINLINE Matrix3x3 operator * (float d, const Matrix3x3 & a)
#endif
{ 
	return a*d; 
}

#ifdef OG
WWINLINE Matrix3 operator / (const Matrix3 & a, float d)
#endif
#ifdef ZH
WWINLINE Matrix3x3 operator / (const Matrix3x3 & a, float d)
#endif
{ 
	float ood = 1.0f / d;
#ifdef OG
	return Matrix3(a.Row[0] * ood, a.Row[1] * ood, a.Row[2] * ood); 
#endif
#ifdef ZH
	return Matrix3x3(a.Row[0] * ood, a.Row[1] * ood, a.Row[2] * ood); 
#endif
}

/*
** matrix addition
*/ 
#ifdef OG
WWINLINE Matrix3 operator + (const Matrix3 & a, const Matrix3 & b)
#endif
#ifdef ZH
WWINLINE Matrix3x3 operator + (const Matrix3x3 & a, const Matrix3x3 & b)
#endif
{
#ifdef OG
	return Matrix3(
#endif
#ifdef ZH
	return Matrix3x3(
#endif
				a.Row[0] + b.Row[0],
				a.Row[1] + b.Row[1],
				a.Row[2] + b.Row[2]
	);
}

#ifdef OG
WWINLINE void Matrix3::Add(const Matrix3 & a, const Matrix3 & b,Matrix3 * c)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Add(const Matrix3x3 & a, const Matrix3x3 & b,Matrix3x3 * c)
#endif
{ 
	WWASSERT(c);
	Vector3::Add(a.Row[0],b.Row[0],&(c->Row[0]));
	Vector3::Add(a.Row[1],b.Row[1],&(c->Row[1]));
	Vector3::Add(a.Row[2],b.Row[2],&(c->Row[2]));
}

/*
** matrix subtraction
*/
#ifdef OG
WWINLINE Matrix3 operator - (const Matrix3 & a, const Matrix3 & b)
#endif
#ifdef ZH
WWINLINE Matrix3x3 operator - (const Matrix3x3 & a, const Matrix3x3 & b)
#endif
{
#ifdef OG
	return Matrix3(
#endif
#ifdef ZH
	return Matrix3x3(
#endif
				a.Row[0] - b.Row[0],
				a.Row[1] - b.Row[1],
				a.Row[2] - b.Row[2]
	);
}

#ifdef OG
WWINLINE void Matrix3::Subtract(const Matrix3 & a, const Matrix3 & b,Matrix3 * c)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Subtract(const Matrix3x3 & a, const Matrix3x3 & b,Matrix3x3 * c)
#endif
{
	WWASSERT(c);
	Vector3::Subtract(a.Row[0],b.Row[0],&(c->Row[0]));
	Vector3::Subtract(a.Row[1],b.Row[1],&(c->Row[1]));
	Vector3::Subtract(a.Row[2],b.Row[2],&(c->Row[2]));
}

/*
** matrix multiplication
*/
#ifdef OG
WWINLINE Matrix3 operator * (const Matrix3 & a, const Matrix3 & b)
#endif
#ifdef ZH
WWINLINE Matrix3x3 operator * (const Matrix3x3 & a, const Matrix3x3 & b)
#endif
{
	#define ROWCOL(i,j) a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j]
    
#ifdef OG
	return Matrix3(
#endif
#ifdef ZH
	return Matrix3x3(
#endif
			Vector3(ROWCOL(0,0), ROWCOL(0,1), ROWCOL(0,2) ),
			Vector3(ROWCOL(1,0), ROWCOL(1,1), ROWCOL(1,2) ),
			Vector3(ROWCOL(2,0), ROWCOL(2,1), ROWCOL(2,2) )
	);
	
	#undef ROWCOL
}


/*
#ifdef OG
** Multiply a Matrix3 by a Vector3. Yeilds a Vector3 result
#endif
#ifdef ZH
** Multiply a Matrix3x3 by a Vector3. Yeilds a Vector3 result
#endif
*/
#ifdef OG
WWINLINE Vector3 operator * (const Matrix3 & a, const Vector3 & v) 
#endif
#ifdef ZH
WWINLINE Vector3 operator * (const Matrix3x3 & a, const Vector3 & v) 
#endif
{
	return Vector3(
		a[0][0] * v[0] + a[0][1] * v[1] + a[0][2] * v[2],
		a[1][0] * v[0] + a[1][1] * v[1] + a[1][2] * v[2],
		a[2][0] * v[0] + a[2][1] * v[1] + a[2][2] * v[2] 
	);
}


#ifdef OG
WWINLINE int operator == (const Matrix3 & a, const Matrix3 & b)
#endif
#ifdef ZH
WWINLINE int operator == (const Matrix3x3 & a, const Matrix3x3 & b)
#endif
{
	return ((a [0] == b [0]) && (a [1] == b [1]) && (a [2] == b [2]));
}


#ifdef OG
WWINLINE int operator != (const Matrix3 & a, const Matrix3 & b)
#endif
#ifdef ZH
WWINLINE int operator != (const Matrix3x3 & a, const Matrix3x3 & b)
#endif
{
	return (!(a == b));
}


/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Rotate_X -- Post-mutiplies an x rotation onto the current matrix                   * 
#endif
#ifdef ZH
 * Matrix3x3::Rotate_X -- Post-mutiplies an x rotation onto the current matrix                   * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   08/26/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE void Matrix3::Rotate_X(float theta)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Rotate_X(float theta)
#endif
{
	Rotate_X(sinf(theta),cosf(theta));
}

#ifdef OG
WWINLINE void Matrix3::Rotate_X(float s,float c)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Rotate_X(float s,float c)
#endif
{
	float tmp1,tmp2;

	tmp1 = Row[0][1]; tmp2 = Row[0][2];
	Row[0][1] = (float)( c*tmp1 + s*tmp2);
	Row[0][2] = (float)(-s*tmp1 + c*tmp2);

	tmp1 = Row[1][1]; tmp2 = Row[1][2];
	Row[1][1] = (float)( c*tmp1 + s*tmp2);
	Row[1][2] = (float)(-s*tmp1 + c*tmp2);

	tmp1 = Row[2][1]; tmp2 = Row[2][2];
	Row[2][1] = (float)( c*tmp1 + s*tmp2);
	Row[2][2] = (float)(-s*tmp1 + c*tmp2);
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Rotate_Y -- Post-multiplies the matrix with a rotation about Y                     * 
#endif
#ifdef ZH
 * Matrix3x3::Rotate_Y -- Post-multiplies the matrix with a rotation about Y                     * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   08/26/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE void Matrix3::Rotate_Y(float theta)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Rotate_Y(float theta)
#endif
{
	Rotate_Y(sinf(theta),cosf(theta));
}

#ifdef OG
WWINLINE void Matrix3::Rotate_Y(float s,float c)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Rotate_Y(float s,float c)
#endif
{
	float tmp1,tmp2;

	tmp1 = Row[0][0]; tmp2 = Row[0][2];
	Row[0][0] = (float)(c*tmp1 - s*tmp2);
	Row[0][2] = (float)(s*tmp1 + c*tmp2);

	tmp1 = Row[1][0]; tmp2 = Row[1][2];
	Row[1][0] = (float)(c*tmp1 - s*tmp2);
	Row[1][2] = (float)(s*tmp1 + c*tmp2);

	tmp1 = Row[2][0]; tmp2 = Row[2][2];
	Row[2][0] = (float)(c*tmp1 - s*tmp2);
	Row[2][2] = (float)(s*tmp1 + c*tmp2);
}


/*********************************************************************************************** 
#ifdef OG
 * Matrix3::Rotate_Z -- Post-multiplies the matrix with a rotation about Z                     * 
#endif
#ifdef ZH
 * Matrix3x3::Rotate_Z -- Post-multiplies the matrix with a rotation about Z                     * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   08/26/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE void Matrix3::Rotate_Z(float theta)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Rotate_Z(float theta)
#endif
{
	Rotate_Z(sinf(theta),cosf(theta));
}

#ifdef OG
WWINLINE void Matrix3::Rotate_Z(float s,float c)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Rotate_Z(float s,float c)
#endif
{
	float tmp1,tmp2;

	tmp1 = Row[0][0]; tmp2 = Row[0][1];
	Row[0][0] = (float)( c*tmp1 + s*tmp2);
	Row[0][1] = (float)(-s*tmp1 + c*tmp2);

	tmp1 = Row[1][0]; tmp2 = Row[1][1];
	Row[1][0] = (float)( c*tmp1 + s*tmp2);
	Row[1][1] = (float)(-s*tmp1 + c*tmp2);

	tmp1 = Row[2][0]; tmp2 = Row[2][1];
	Row[2][0] = (float)( c*tmp1 + s*tmp2);
	Row[2][1] = (float)(-s*tmp1 + c*tmp2);
}


/*********************************************************************************************** 
 * Create_X_Rotation_Matrix3 -- creates a matrix3 which is a rotation about X                  * 
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   08/26/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3 Create_X_Rotation_Matrix3(float s,float c)
#endif
#ifdef ZH
WWINLINE Matrix3x3 Create_X_Rotation_Matrix3(float s,float c)
#endif
{
#ifdef OG
	Matrix3 mat;
#endif
#ifdef ZH
	Matrix3x3 mat;
#endif

	mat[0][0] = 1.0f;
	mat[0][1] = 0.0f;
	mat[0][2] = 0.0f;

	mat[1][0] = 0.0f;
	mat[1][1] = c;
	mat[1][2] = -s;

	mat[2][0] = 0.0f;
	mat[2][1] = s;
	mat[2][2] = c;

	return mat;
}

#ifdef OG
WWINLINE Matrix3 Create_X_Rotation_Matrix3(float rad)
#endif
#ifdef ZH
WWINLINE Matrix3x3 Create_X_Rotation_Matrix3(float rad)
#endif
{
	return Create_X_Rotation_Matrix3(sinf(rad),cosf(rad));
}

/*********************************************************************************************** 
#ifdef OG
 * Create_Y_Rotation_Matrix3 -- Creates a Matrix3 which is a rotation about Y                  * 
#endif
#ifdef ZH
 * Create_Y_Rotation_Matrix3 -- Creates a Matrix3x3 which is a rotation about Y                  * 
#endif
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   08/26/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3 Create_Y_Rotation_Matrix3(float s,float c)
#endif
#ifdef ZH
WWINLINE Matrix3x3 Create_Y_Rotation_Matrix3(float s,float c)
#endif
{
#ifdef OG
	Matrix3 mat;
#endif
#ifdef ZH
	Matrix3x3 mat;
#endif

	mat[0][0] = c;
	mat[0][1] = 0.0f;
	mat[0][2] = s;

	mat[1][0] = 0.0f;
	mat[1][1] = 1.0f;
	mat[1][2] = 0.0f;

	mat[2][0] = -s;
	mat[2][1] = 0.0f;
	mat[2][2] = c;

	return mat;
}

#ifdef OG
WWINLINE Matrix3 Create_Y_Rotation_Matrix3(float rad)
#endif
#ifdef ZH
WWINLINE Matrix3x3 Create_Y_Rotation_Matrix3(float rad)
#endif
{
	return Create_Y_Rotation_Matrix3(sinf(rad),cosf(rad));
}

/*********************************************************************************************** 
 * Create_Z_Rotation_Matrix3 -- Creates a matrix3 which is a rotation about Z                  * 
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   08/26/1997 GH  : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
WWINLINE Matrix3 Create_Z_Rotation_Matrix3(float s,float c)
#endif
#ifdef ZH
WWINLINE Matrix3x3 Create_Z_Rotation_Matrix3(float s,float c)
#endif
{
#ifdef OG
	Matrix3 mat;
#endif
#ifdef ZH
	Matrix3x3 mat;
#endif

	mat[0][0] = c;
	mat[0][1] = -s;
	mat[0][2] = 0.0f;

	mat[1][0] = s;
	mat[1][1] = c;
	mat[1][2] = 0.0f;

	mat[2][0] = 0.0f;
	mat[2][1] = 0.0f;
	mat[2][2] = 1.0f;

	return mat;
}

#ifdef OG
WWINLINE Matrix3 Create_Z_Rotation_Matrix3(float rad)
#endif
#ifdef ZH
WWINLINE Matrix3x3 Create_Z_Rotation_Matrix3(float rad)
#endif
{
	return Create_Z_Rotation_Matrix3(sinf(rad),cosf(rad));
}

#ifdef OG
WWINLINE void Matrix3::Rotate_Vector(const Matrix3 & A,const Vector3 & in,Vector3 * out)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Rotate_Vector(const Matrix3x3 & A,const Vector3 & in,Vector3 * out)
#endif
{
	Vector3 tmp;
	Vector3 * v;

	// check for aliased parameters
	if (out == &in) {
		tmp = in;
		v = &tmp;
	} else {
		v = (Vector3 *)&in;		// whats the right way to do this...
	}

	out->X = (A[0][0] * v->X + A[0][1] * v->Y + A[0][2] * v->Z);
	out->Y = (A[1][0] * v->X + A[1][1] * v->Y + A[1][2] * v->Z);
	out->Z = (A[2][0] * v->X + A[2][1] * v->Y + A[2][2] * v->Z);
}

#ifdef OG
WWINLINE void Matrix3::Transpose_Rotate_Vector(const Matrix3 & A,const Vector3 & in,Vector3 * out)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Transpose_Rotate_Vector(const Matrix3x3 & A,const Vector3 & in,Vector3 * out)
#endif
{
	Vector3 tmp;
	Vector3 * v;

	// check for aliased parameters
	if (out == &in) {
		tmp = in;
		v = &tmp;
	} else {
		v = (Vector3 *)&in;
	}
	out->X = (A[0][0] * v->X + A[1][0] * v->Y + A[2][0] * v->Z);
	out->Y = (A[0][1] * v->X + A[1][1] * v->Y + A[2][1] * v->Z);
	out->Z = (A[0][2] * v->X + A[1][2] * v->Y + A[2][2] * v->Z);
}

#ifdef OG
WWINLINE void Matrix3::Rotate_AABox_Extent(const Vector3 & extent,Vector3 * set_extent)
#endif
#ifdef ZH
WWINLINE void Matrix3x3::Rotate_AABox_Extent(const Vector3 & extent,Vector3 * set_extent)
#endif
{
	// push each extent out to the projections of the original extents
	for (int i=0; i<3; i++) {

		// start the center out at the translation portion of the matrix
		// and the extent at zero
		(*set_extent)[i] = 0.0f;

		for (int j=0; j<3; j++) {
			(*set_extent)[i] += WWMath::Fabs(Row[i][j] * extent[j]);
		}
	}
}

#endif /*Matrix3_H*/
