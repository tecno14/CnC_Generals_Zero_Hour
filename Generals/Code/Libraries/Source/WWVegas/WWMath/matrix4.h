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

#ifdef OG
/* $Header: /Commando/Code/wwmath/matrix4.h 19    5/11/01 7:11p Jani_p $ */
#endif
#ifdef ZH
/* $Header: /Commando/Code/wwmath/matrix4.h 20    10/04/01 10:33a Greg_h $ */
#endif
/*********************************************************************************************** 
 ***                            Confidential - Westwood Studios                              *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : WW3D                                                         * 
 *                                                                                             * 
 *                    File Name : MATRIX4.H                                                    * 
#ifdef ZH
 *                                                                                             * 
 *               Org Programmer : Greg Hjelstrom                                               * 
#endif
 *                                                                                             * 
#ifdef OG
 *                   Programmer : Greg Hjelstrom                                               * 
#endif
#ifdef ZH
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
 *   Matrix4::Matrix4 -- Constructor, optionally initialize to Identitiy matrix                * 
 *   Matrix4::Matrix4 -- Copy Constructor                                                      * 
 *   Matrix4::Matrix4 -- Convert a Matrix3D (fake 4x4) to a Matrix4                            * 
 *   Matrix4::Matrix4 -- Constructor                                                           * 
 *   Matrix4::Make_Identity -- Initializes the matrix to Identity                              *
 *   Matrix4::Init -- Initializes from the contents of the give Matrix3D                       *
 *   Matrix4::Init -- Initializes the rows from the given Vector4s                             *
 *   Matrix4::Init_Ortho -- Initialize to an orthographic projection matrix                    *
 *   Matrix4::Init_Perspective -- Initialize to a perspective projection matrix                *
 *   Matrix4::Init_Perspective -- Initialize to a perspective projection matrix                *
 *   Matrix4::Transpose -- Returns transpose of the matrix                                     * 
 *   Matrix4::Inverse -- returns the inverse of the matrix                                     * 
 *   Matrix4::operator = -- assignment operator                                                * 
 *   Matrix4::operator += -- "plus equals" operator                                            * 
 *   Matrix4::operator-= -- "minus equals" operator                                            * 
 *   Matrix4::operator *= -- "times equals" operator                                           * 
 *   Matrix4::operator /= -- "divide equals" operator                                          * 

#endif
#ifdef ZH
 *   Matrix4x4::Matrix4x4 -- Constructor, optionally initialize to Identitiy matrix                * 
 *   Matrix4x4::Matrix4x4 -- Copy Constructor                                                      * 
 *   Matrix4x4::Matrix4x4 -- Convert a Matrix3D (fake 4x4) to a Matrix4x4                            * 
 *   Matrix4x4::Matrix4x4 -- Constructor                                                           * 
 *   Matrix4x4::Make_Identity -- Initializes the matrix to Identity                              *
 *   Matrix4x4::Init -- Initializes from the contents of the give Matrix3D                       *
 *   Matrix4x4::Init -- Initializes the rows from the given Vector4s                             *
 *   Matrix4x4::Init -- Initializes the rows from the given 16 floats                            *
 *   Matrix4x4::Init_Ortho -- Initialize to an orthographic projection matrix                    *
 *   Matrix4x4::Init_Perspective -- Initialize to a perspective projection matrix                *
 *   Matrix4x4::Init_Perspective -- Initialize to a perspective projection matrix                *
 *   Matrix4x4::Transpose -- Returns transpose of the matrix                                     * 
 *   Matrix4x4::Inverse -- returns the inverse of the matrix                                     * 
 *   Matrix4x4::operator = -- assignment operator                                                * 
 *   Matrix4x4::operator += -- "plus equals" operator                                            * 
 *   Matrix4x4::operator -= -- "minus equals" operator                                            * 
 *   Matrix4x4::operator *= -- "times equals" operator                                           * 
 *   Matrix4x4::operator /= -- "divide equals" operator                                          * 
#endif
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#if defined(_MSC_VER)
#pragma once
#endif


#ifndef MATRIX4_H
#define MATRIX4_H

#include "always.h"
#include "vector4.h"
#include "matrix3d.h"
#include "matrix3.h"


#ifdef OG
class Matrix4
#endif
#ifdef ZH
class Matrix4x4
#endif
{
public:

	/*
	** Constructors
	*/
#ifdef OG
	Matrix4(void) {};
	Matrix4(const Matrix4 & m);
#endif
#ifdef ZH
	Matrix4x4(void) {};
	Matrix4x4(const Matrix4x4 & m);
#endif

#ifdef OG
	WWINLINE explicit Matrix4(bool identity);
	WWINLINE explicit Matrix4(const Matrix3D & m);
	WWINLINE explicit Matrix4(const Matrix3 & m);
	WWINLINE explicit Matrix4(const Vector4 & v0, const Vector4 & v1, const Vector4 & v2, const Vector4 & v3);

#endif
#ifdef ZH
	WWINLINE explicit Matrix4x4(bool identity);
	WWINLINE explicit Matrix4x4(const Matrix3D & m);
	WWINLINE explicit Matrix4x4(const Matrix3x3 & m);
	WWINLINE explicit Matrix4x4(const Vector4 & v0, const Vector4 & v1, const Vector4 & v2, const Vector4 & v3);
	WWINLINE explicit Matrix4x4(	float m11,float m12,float m13,float m14,
										float m21,float m22,float m23,float m24,
										float m31,float m32,float m33,float m34,  
										float m41,float m42,float m43,float m44 ); 
#endif
	
	WWINLINE void		Make_Identity(void);
	WWINLINE void		Init(const Matrix3D & m);
#ifdef OG
	WWINLINE void		Init(const Matrix3 & m);
#endif
#ifdef ZH
	WWINLINE void		Init(const Matrix3x3 & m);
#endif
	WWINLINE void		Init(const Vector4 & v0, const Vector4 & v1, const Vector4 & v2, const Vector4 & v3);
#ifdef ZH
	WWINLINE void		Init(	float m11,float m12,float m13,float m14,
									float m21,float m22,float m23,float m24,
									float m31,float m32,float m33,float m34,  
									float m41,float m42,float m43,float m44 ); 
#endif

	/*
	** Projection matrices.  The znear and zfar parameters are positive values indicating the
	** distance from the camera to the z clipping planes.  See implementations for more info.
	*/
	WWINLINE void		Init_Ortho(float left,float right,float bottom,float top,float znear,float zfar);
	WWINLINE void		Init_Perspective(float hfov,float vfov,float znear,float zfar);
	WWINLINE void		Init_Perspective(float left,float right,float bottom,float top,float znear,float zfar);

	/*
	** Access operators
	*/
	WWINLINE Vector4 & operator [] (int i) { return Row[i]; }
	WWINLINE const Vector4 & operator [] (int i) const { return Row[i]; }

	/*
	** Transpose and Inverse
	*/
#ifdef OG
	WWINLINE Matrix4 Transpose(void) const;
	WWINLINE Matrix4 Inverse(void) const;
#endif
#ifdef ZH
	WWINLINE Matrix4x4 Transpose(void) const;
	WWINLINE Matrix4x4 Inverse(void) const;
#endif

	/*
	** Assignment operators
	*/
#ifdef OG
	WWINLINE Matrix4 & operator = (const Matrix4 & m);
	WWINLINE Matrix4 & operator += (const Matrix4 & m);
	WWINLINE Matrix4 & operator -= (const Matrix4 & m);
	WWINLINE Matrix4 & operator *= (float d);
	WWINLINE Matrix4 & operator /= (float d);
#endif
#ifdef ZH
	WWINLINE Matrix4x4 & operator = (const Matrix4x4 & m);
	WWINLINE Matrix4x4 & operator += (const Matrix4x4 & m);
	WWINLINE Matrix4x4 & operator -= (const Matrix4x4 & m);
	WWINLINE Matrix4x4 & operator *= (float d);
	WWINLINE Matrix4x4 & operator /= (float d);
#endif

	/*
	** Negation
	*/
#ifdef OG
	WWINLINE friend Matrix4 operator - (const Matrix4& a);
#endif
#ifdef ZH
	WWINLINE friend Matrix4x4 operator - (const Matrix4x4& a);
#endif
	
	/*
	** Scalar multiplication and division
	*/
#ifdef OG
	WWINLINE friend Matrix4 operator * (const Matrix4& a,float d);
	WWINLINE friend Matrix4 operator * (float d,const Matrix4& a);
	WWINLINE friend Matrix4 operator / (const Matrix4& a,float d);
#endif
#ifdef ZH
	WWINLINE friend Matrix4x4 operator * (const Matrix4x4& a,float d);
	WWINLINE friend Matrix4x4 operator * (float d,const Matrix4x4& a);
	WWINLINE friend Matrix4x4 operator / (const Matrix4x4& a,float d);
#endif

	/*
	** matrix addition
	*/ 
#ifdef OG
	WWINLINE friend Matrix4 operator + (const Matrix4& a, const Matrix4& b);
	WWINLINE friend Matrix4 Add(const Matrix4& a);
#endif
#ifdef ZH
	WWINLINE friend Matrix4x4 operator + (const Matrix4x4& a, const Matrix4x4& b);
	WWINLINE friend Matrix4x4 Add(const Matrix4x4& a);
#endif

	/*
	** matrix subtraction
	*/
#ifdef OG
	WWINLINE friend Matrix4 operator - (const Matrix4 & a, const Matrix4 & b);
	WWINLINE friend Matrix4 Subtract(const Matrix4 & a, const Matrix4 & b);
#endif
#ifdef ZH
	WWINLINE friend Matrix4x4 operator - (const Matrix4x4 & a, const Matrix4x4 & b);
	WWINLINE friend Matrix4x4 Subtract(const Matrix4x4 & a, const Matrix4x4 & b);
#endif

	/*
	** matrix multiplication
	*/
#ifdef OG
	WWINLINE friend Matrix4 operator * (const Matrix4 & a, const Matrix4 & b);
	WWINLINE friend Matrix4 Multiply(const Matrix4 & a, const Matrix4 & b);
	WWINLINE friend Matrix4 operator * (const Matrix4 & a, const Matrix3D & b);
	WWINLINE friend Matrix4 operator * (const Matrix3D & a, const Matrix4 & b);
#endif
#ifdef ZH
	WWINLINE friend Matrix4x4 operator * (const Matrix4x4 & a, const Matrix4x4 & b);
	WWINLINE friend Matrix4x4 Multiply(const Matrix4x4 & a, const Matrix4x4 & b);
	WWINLINE friend Matrix4x4 operator * (const Matrix4x4 & a, const Matrix3D & b);
	WWINLINE friend Matrix4x4 operator * (const Matrix3D & a, const Matrix4x4 & b);
#endif

	/*
	** Comparison operators
	*/
#ifdef OG
	friend int operator == (const Matrix4 & a, const Matrix4 & b);
	friend int operator != (const Matrix4 & a, const Matrix4 & b);
#endif
#ifdef ZH
	friend int operator == (const Matrix4x4 & a, const Matrix4x4 & b);
	friend int operator != (const Matrix4x4 & a, const Matrix4x4 & b);
#endif

	/*
	** Swap two matrices in place
	*/
#ifdef OG
	WWINLINE friend void Swap(Matrix4 & a,Matrix4 & b);
#endif
#ifdef ZH
	WWINLINE friend void Swap(Matrix4x4 & a,Matrix4x4 & b);
#endif

	/*
	** Linear Transforms
	*/
#ifdef OG
	WWINLINE friend Vector4 operator * (const Matrix4 & a, const Vector4 & v);
	WWINLINE friend Vector4 operator * (const Matrix4 & a, const Vector3 & v);
#endif
#ifdef ZH
	WWINLINE friend Vector4 operator * (const Matrix4x4 & a, const Vector4 & v);
	WWINLINE friend Vector4 operator * (const Matrix4x4 & a, const Vector3 & v);
#endif

	/*
	** Matrix multiplication without temporaries...
	*/
#ifdef OG
	static void	Multiply(const Matrix4 &A,const Matrix4 &B,Matrix4 * set_result);
	static void	Multiply(const Matrix3D &A,const Matrix4 &B,Matrix4 * set_result);
	static void	Multiply(const Matrix4 &A,const Matrix3D &B,Matrix4 * set_result);
#endif
#ifdef ZH
	static void	Multiply(const Matrix4x4 &A,const Matrix4x4 &B,Matrix4x4 * set_result);
	static void	Multiply(const Matrix3D &A,const Matrix4x4 &B,Matrix4x4 * set_result);
	static void	Multiply(const Matrix4x4 &A,const Matrix3D &B,Matrix4x4 * set_result);
#endif

#ifdef OG
	static WWINLINE void	Transform_Vector(const Matrix4 & tm,const Vector3 & in,Vector3 * out);
	static WWINLINE void	Transform_Vector(const Matrix4 & tm,const Vector3 & in,Vector4 * out);
	static WWINLINE void	Transform_Vector(const Matrix4 & tm,const Vector4 & in,Vector4 * out);
#endif
#ifdef ZH
	static WWINLINE void	Transform_Vector(const Matrix4x4 & tm,const Vector3 & in,Vector3 * out);
	static WWINLINE void	Transform_Vector(const Matrix4x4 & tm,const Vector3 & in,Vector4 * out);
	static WWINLINE void	Transform_Vector(const Matrix4x4 & tm,const Vector4 & in,Vector4 * out);
#endif

protected:

	Vector4 Row[4];

};


/*********************************************************************************************** 
#ifdef OG
 * Matrix4::Matrix4 -- Constructor, optionally initialize to Identitiy matrix                  * 
#endif
#ifdef ZH
 * Matrix4x4::Matrix4x4 -- Constructor, optionally initialize to Identitiy matrix                  * 
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
WWINLINE Matrix4::Matrix4(bool identity)
#endif
#ifdef ZH
WWINLINE Matrix4x4::Matrix4x4(bool identity)
#endif
{
	if (identity) {
		Make_Identity();
	}
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix4::Matrix4 -- Copy Constructor                                                        * 
#endif
#ifdef ZH
 * Matrix4x4::Matrix4x4 -- Copy Constructor                                                        * 
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
WWINLINE Matrix4::Matrix4(const Matrix4 & m)
#endif
#ifdef ZH
WWINLINE Matrix4x4::Matrix4x4(const Matrix4x4 & m)
#endif
{
	Row[0] = m.Row[0]; Row[1] = m.Row[1]; Row[2] = m.Row[2]; Row[3] = m.Row[3]; 
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix4::Matrix4 -- Convert a Matrix3D (fake 4x4) to a Matrix4                              * 
#endif
#ifdef ZH
 * Matrix4x4::Matrix4x4 -- Convert a Matrix3D (fake 4x4) to a Matrix4x4                              * 
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
WWINLINE Matrix4::Matrix4(const Matrix3D & m)
#endif
#ifdef ZH
WWINLINE Matrix4x4::Matrix4x4(const Matrix3D & m)
#endif
{
	Init(m);
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix4::Matrix4 -- Constructor                                                             * 
#endif
#ifdef ZH
 * Matrix4x4::Matrix4x4 -- Constructor                                                             * 
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
WWINLINE Matrix4::Matrix4(const Vector4 & r0, const Vector4 & r1, const Vector4 & r2, const Vector4 & r3)
#endif
#ifdef ZH
WWINLINE Matrix4x4::Matrix4x4(const Vector4 & r0, const Vector4 & r1, const Vector4 & r2, const Vector4 & r3)
#endif
{ 
	Init(r0,r1,r2,r3);
}

#ifdef ZH
/*********************************************************************************************** 
 * Matrix4x4::Matrix4x4 -- Constructor                                                             * 
 *                                                                                             * 
 * INPUT:                                                                                      * 
 *                                                                                             * 
 * OUTPUT:                                                                                     * 
 *                                                                                             * 
 * WARNINGS:                                                                                   * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   11/06/2001 NH  : Created.                                                                 * 
 *=============================================================================================*/
WWINLINE Matrix4x4::Matrix4x4(float m11,float m12,float m13,float m14, float m21,float m22,float m23,float m24,
	float m31,float m32,float m33,float m34, float m41,float m42,float m43,float m44 )
{
	Init(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
}
#endif

/***********************************************************************************************
#ifdef OG
 * Matrix4::Make_Identity -- Initializes the matrix to Identity                                *
#endif
#ifdef ZH
 * Matrix4x4::Make_Identity -- Initializes the matrix to Identity                                *
#endif
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/5/99    gth : Created.                                                                 *
 *=============================================================================================*/
#ifdef OG
WWINLINE void Matrix4::Make_Identity(void)
#endif
#ifdef ZH
WWINLINE void Matrix4x4::Make_Identity(void)
#endif
{
	Row[0].Set(1.0,0.0,0.0,0.0);
	Row[1].Set(0.0,1.0,0.0,0.0);
	Row[2].Set(0.0,0.0,1.0,0.0);
	Row[3].Set(0.0,0.0,0.0,1.0);
}


/***********************************************************************************************
#ifdef OG
 * Matrix4::Init -- Initializes from the contents of the give Matrix3D                         *
#endif
#ifdef ZH
 * Matrix4x4::Init -- Initializes from the contents of the give Matrix3D                         *
#endif
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/5/99    gth : Created.                                                                 *
 *=============================================================================================*/
#ifdef OG
WWINLINE void Matrix4::Init(const Matrix3D & m)
#endif
#ifdef ZH
WWINLINE void Matrix4x4::Init(const Matrix3D & m)
#endif
{
	Row[0] = m[0]; Row[1] = m[1]; Row[2] = m[2]; Row[3] = Vector4(0.0,0.0,0.0,1.0); 
}


/***********************************************************************************************
#ifdef OG
 * Matrix4::Init -- Initializes the rows from the given Vector4s                               *
#endif
#ifdef ZH
 * Matrix4x4::Init -- Initializes the rows from the given Vector4s                               *
#endif
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/5/99    gth : Created.                                                                 *
 *=============================================================================================*/
#ifdef OG
WWINLINE void Matrix4::Init(const Vector4 & r0, const Vector4 & r1, const Vector4 & r2, const Vector4 & r3)
#endif
#ifdef ZH
WWINLINE void Matrix4x4::Init(const Vector4 & r0, const Vector4 & r1, const Vector4 & r2, const Vector4 & r3)
#endif
{
	Row[0] = r0; Row[1] = r1; Row[2] = r2; Row[3] = r3; 
}


/***********************************************************************************************
#ifdef OG
 * Matrix4::Init_Ortho -- Initialize to an orthographic projection matrix                      *

#endif
#ifdef ZH
 * Matrix4x4::Init -- Initializes the rows from the given 16 floats                              *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/6/01    NH : Created.                                                                  *
 *=============================================================================================*/
WWINLINE void Matrix4x4::Init(float m11,float m12,float m13,float m14, float m21,float m22,float m23,float m24,
	float m31,float m32,float m33,float m34, float m41,float m42,float m43,float m44)
{
	Row[0].Set(m11,m12,m13,m14);
	Row[1].Set(m21,m22,m23,m24);
	Row[2].Set(m31,m32,m33,m34);
	Row[3].Set(m41,m42,m43,m44);
}

/***********************************************************************************************
 * Matrix4x4::Init_Ortho -- Initialize to an orthographic projection matrix                      *
#endif
 *                                                                                             *
 * You can find the formulas for this in the appendix of the OpenGL programming guide.  Also   *
 * this happens to be the same convention used by Surrender.                                   *
 *                                                                                             *
 * The result of this projection will be that points inside the volume will have all coords    *
 * between -1 and +1.  A point at znear will project to z=-1.  A point at zfar will project    *
 * to z=+1...                                                                                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * Note that the znear and zfar parameters are positive distances to the clipping planes       *
 * even though in the camera coordinate system, the clipping planes are at negative z          *
 * coordinates.  This holds for all of the projection initializations and is consistent        *
 * with OpenGL's convention.                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/5/99    gth : Created.                                                                 *
 *=============================================================================================*/
#ifdef OG
WWINLINE void Matrix4::Init_Ortho
#endif
#ifdef ZH
WWINLINE void Matrix4x4::Init_Ortho
#endif
(
	float left,
	float right,
	float bottom,
	float top,
	float znear,
	float zfar
)
{
	assert(znear >= 0.0f);
	assert(zfar > znear);

	Make_Identity();
	Row[0][0] = 2.0f / (right - left);
	Row[0][3] = -(right + left) / (right - left);
	Row[1][1] = 2.0f / (top - bottom);
	Row[1][3] = -(top + bottom) / (top - bottom);
	Row[2][2] = -2.0f / (zfar - znear);
	Row[2][3] = -(zfar + znear) / (zfar - znear);
}


/***********************************************************************************************
#ifdef OG
 * Matrix4::Init_Perspective -- Initialize to a perspective projection matrix                  *
#endif
#ifdef ZH
 * Matrix4x4::Init_Perspective -- Initialize to a perspective projection matrix                  *
#endif
 *                                                                                             *
 * You can find the formulas for this matrix in the appendix of the OpenGL programming guide.  *
 * Also, this happens to be the same convention used by Surrender.                             *
 *                                                                                             *
 * The result of this projection will be that points inside the volume will have all coords    *
 * between -1 and +1.  A point at znear will project to z=-1.  A point at zfar will project    *
 * to z=+1...                                                                                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 * hfov - horizontal field of view (in radians)                                                *
 * vfov - vertical field of view (in radians)                                                  *
 * znear - distance to near z clipping plane (positive)                                        *
 * zfar - distance to the far z clipping plane (positive)                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * Note that the znear and zfar parameters are positive distances to the clipping planes       *
 * even though in the camera coordinate system, the clipping planes are at negative z          *
 * coordinates.  This holds for all of the projection initializations and is consistent        *
 * with OpenGL's convention.                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/5/99    gth : Created.                                                                 *
 *=============================================================================================*/
#ifdef OG
WWINLINE void Matrix4::Init_Perspective(float hfov,float vfov,float znear,float zfar)
#endif
#ifdef ZH
WWINLINE void Matrix4x4::Init_Perspective(float hfov,float vfov,float znear,float zfar)
#endif
{
	assert(znear > 0.0f);
	assert(zfar > znear);

	Make_Identity();
	Row[0][0] = static_cast<float>(1.0 / tan(hfov*0.5));
	Row[1][1] = static_cast<float>(1.0 / tan(vfov*0.5));
	Row[2][2] = -(zfar + znear) / (zfar - znear);
	Row[2][3] = static_cast<float>(-(2.0*zfar*znear) / (zfar - znear));
	Row[3][2] = -1.0f;
	Row[3][3] = 0.0f;
}


/***********************************************************************************************
#ifdef OG
 * Matrix4::Init_Perspective -- Initialize to a perspective projection matrix                  *
#endif
#ifdef ZH
 * Matrix4x4::Init_Perspective -- Initialize to a perspective projection matrix                  *
#endif
 *                                                                                             *
 * You can find the formulas for this matrix in the appendix of the OpenGL programming guide.  *
 * Also, this happens to be the same convention used by Surrender.                             *
 *                                                                                             *
 * The result of this projection will be that points inside the volume will have all coords    *
 * between -1 and +1.  A point at znear will project to z=-1.  A point at zfar will project    *
 * to z=+1...                                                                                  *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * left - min x coordinate of near clip plane                                                  *
 * right - max x coordinate of near clip plane                                                 *
 * bottom - min y coordinate of near clip plane                                                *
 * top - max y coordinate of near clip plane                                                   *
 * znear - distance to near Z clipping plane                                                   *
 * zfar - distance to far Z clipping plane                                                     *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * Note that the znear and zfar parameters are positive distances to the clipping planes       *
 * even though in the camera coordinate system, the clipping planes are at negative z          *
 * coordinates.  This holds for all of the projection initializations and is consistent        *
 * with OpenGL's convention.                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/5/99    gth : Created.                                                                 *
 *=============================================================================================*/
#ifdef OG
WWINLINE void Matrix4::Init_Perspective
#endif
#ifdef ZH
WWINLINE void Matrix4x4::Init_Perspective
#endif
(
	float left,
	float right,
	float bottom,
	float top,
	float znear,
	float zfar
)
{
	assert(znear > 0.0f);
	assert(zfar > 0.0f);

	Make_Identity();
	Row[0][0] = static_cast<float>(2.0*znear / (right - left));
	Row[0][2] = (right + left) / (right - left);
	Row[1][1] = static_cast<float>(2.0*znear / (top - bottom));
	Row[1][2] = (top + bottom) / (top - bottom);
	Row[2][2] = -(zfar + znear) / (zfar - znear);
	Row[2][3] = static_cast<float>(-(2.0*zfar*znear) / (zfar - znear));
	Row[3][2] = -1.0f;
	Row[3][3] = 0.0f;
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix4::Transpose -- Returns transpose of the matrix                                       * 
#endif
#ifdef ZH
 * Matrix4x4::Transpose -- Returns transpose of the matrix                                       * 
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
WWINLINE Matrix4 Matrix4::Transpose() const
#endif
#ifdef ZH
WWINLINE Matrix4x4 Matrix4x4::Transpose() const
#endif
{
#ifdef OG
    return Matrix4(
#endif
#ifdef ZH
    return Matrix4x4(
#endif
			Vector4(Row[0][0], Row[1][0], Row[2][0], Row[3][0]),
			Vector4(Row[0][1], Row[1][1], Row[2][1], Row[3][1]),
			Vector4(Row[0][2], Row[1][2], Row[2][2], Row[3][2]),
			Vector4(Row[0][3], Row[1][3], Row[2][3], Row[3][3])
	);
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix4::Inverse -- returns the inverse of the matrix                                       * 
#endif
#ifdef ZH
 * Matrix4x4::Inverse -- returns the inverse of the matrix                                       * 
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
WWINLINE Matrix4 Matrix4::Inverse() const    // Gauss-Jordan elimination with partial pivoting
#endif
#ifdef ZH
WWINLINE Matrix4x4 Matrix4x4::Inverse() const    // Gauss-Jordan elimination with partial pivoting
#endif
{
#ifdef OG
	Matrix4 a(*this);				// As a evolves from original mat into identity
	Matrix4 b(true);				// b evolves from identity into inverse(a)

#endif
#ifdef ZH
	WWASSERT_PRINT(0,"Matrix4x4::Inverse does not work, re-implement!");

	Matrix4x4 a(*this);				// As a evolves from original mat into identity
	Matrix4x4 b(true);				// b evolves from identity into inverse(a)
#endif
	int i, j, i1;

	// Loop over cols of a from left to right, eliminating above and below diagonal
	for (j=0; j<4; j++) {

		// Find largest pivot in column j among rows j..3
		i1 = j;
		for (i=j+1; i<4; i++) {
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
			//ALGEBRA_ERROR("Matrix4::inverse: singular matrix; can't invert\n");
#endif
#ifdef ZH
			//ALGEBRA_ERROR("Matrix4x4::inverse: singular matrix; can't invert\n");
#endif
		}
		b.Row[j] /= a.Row[j][j];
		a.Row[j] /= a.Row[j][j];

		// Eliminate off-diagonal elems in col j of a, doing identical ops to b
		for (i=0; i<4; i++) {
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
 * Matrix4::operator = -- assignment operator                                                  * 
#endif
#ifdef ZH
 * Matrix4x4::operator = -- assignment operator                                                  * 
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
WWINLINE Matrix4 & Matrix4::operator = (const Matrix4 & m)
#endif
#ifdef ZH
WWINLINE Matrix4x4 & Matrix4x4::operator = (const Matrix4x4 & m)
#endif
{
	Row[0] = m.Row[0]; Row[1] = m.Row[1]; Row[2] = m.Row[2]; Row[3] = m.Row[3];
	return *this; 
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix4::operator += -- "plus equals" operator                                              * 
#endif
#ifdef ZH
 * Matrix4x4::operator += -- "plus equals" operator                                              * 
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
WWINLINE Matrix4& Matrix4::operator += (const Matrix4 & m)
#endif
#ifdef ZH
WWINLINE Matrix4x4& Matrix4x4::operator += (const Matrix4x4 & m)
#endif
{
	Row[0] += m.Row[0]; Row[1] += m.Row[1]; Row[2] += m.Row[2]; Row[3] += m.Row[3];
	return *this; 
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix4::operator-= -- "minus equals" operator                                              * 
#endif
#ifdef ZH
 * Matrix4x4::operator-= -- "minus equals" operator                                              * 
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
WWINLINE Matrix4& Matrix4::operator -= (const Matrix4 & m)
#endif
#ifdef ZH
WWINLINE Matrix4x4& Matrix4x4::operator -= (const Matrix4x4 & m)
#endif
{
	Row[0] -= m.Row[0]; Row[1] -= m.Row[1]; Row[2] -= m.Row[2]; Row[3] -= m.Row[3];
	return *this; 
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix4::operator *= -- "times equals" operator                                             * 
#endif
#ifdef ZH
 * Matrix4x4::operator *= -- "times equals" operator                                             * 
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
WWINLINE Matrix4& Matrix4::operator *= (float d)
#endif
#ifdef ZH
WWINLINE Matrix4x4& Matrix4x4::operator *= (float d)
#endif
{
	Row[0] *= d; Row[1] *= d; Row[2] *= d; Row[3] *= d;
	return *this; 
}

/*********************************************************************************************** 
#ifdef OG
 * Matrix4::operator /= -- "divide equals" operator                                            * 
#endif
#ifdef ZH
 * Matrix4x4::operator /= -- "divide equals" operator                                            * 
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
WWINLINE Matrix4& Matrix4::operator /= (float d)
#endif
#ifdef ZH
WWINLINE Matrix4x4& Matrix4x4::operator /= (float d)
#endif
{
	float ood = d;
	Row[0] *= ood; Row[1] *= ood; Row[2] *= ood; Row[3] *= ood;
	return *this; 
}

#ifdef OG
WWINLINE Matrix4 operator - (const Matrix4 & a)
#endif
#ifdef ZH
WWINLINE Matrix4x4 operator - (const Matrix4x4 & a)
#endif
{ 
#ifdef OG
	return Matrix4(-a.Row[0], -a.Row[1], -a.Row[2], -a.Row[3]); 
#endif
#ifdef ZH
	return Matrix4x4(-a.Row[0], -a.Row[1], -a.Row[2], -a.Row[3]); 
#endif
}

#ifdef OG
WWINLINE Matrix4 operator * (const Matrix4 & a, float d)
#endif
#ifdef ZH
WWINLINE Matrix4x4 operator * (const Matrix4x4 & a, float d)
#endif
{ 
#ifdef OG
	return Matrix4(a.Row[0] * d, a.Row[1] * d, a.Row[2] * d, a.Row[3] * d); 
#endif
#ifdef ZH
	return Matrix4x4(a.Row[0] * d, a.Row[1] * d, a.Row[2] * d, a.Row[3] * d); 
#endif
}

#ifdef OG
WWINLINE Matrix4 operator * (float d, const Matrix4 & a)
#endif
#ifdef ZH
WWINLINE Matrix4x4 operator * (float d, const Matrix4x4 & a)
#endif
{ 
	return a*d; 
}

#ifdef OG
WWINLINE Matrix4 operator / (const Matrix4 & a, float d)
#endif
#ifdef ZH
WWINLINE Matrix4x4 operator / (const Matrix4x4 & a, float d)
#endif
{ 
	float ood = 1.0f / d;
#ifdef OG
	return Matrix4(a.Row[0] * ood, a.Row[1] * ood, a.Row[2] * ood, a.Row[3] * ood); 
#endif
#ifdef ZH
	return Matrix4x4(a.Row[0] * ood, a.Row[1] * ood, a.Row[2] * ood, a.Row[3] * ood); 
#endif
}

/*
** matrix addition
*/ 
#ifdef OG
WWINLINE Matrix4 operator + (const Matrix4 & a, const Matrix4 & b)
#endif
#ifdef ZH
WWINLINE Matrix4x4 operator + (const Matrix4x4 & a, const Matrix4x4 & b)
#endif
{
#ifdef OG
	return Matrix4(
#endif
#ifdef ZH
	return Matrix4x4(
#endif
				a.Row[0] + b.Row[0],
				a.Row[1] + b.Row[1],
				a.Row[2] + b.Row[2],
				a.Row[3] + b.Row[3]
	);
}

#ifdef OG
WWINLINE Matrix4 Add(const Matrix4 & a, const Matrix4 & b)
#endif
#ifdef ZH
WWINLINE Matrix4x4 Add(const Matrix4x4 & a, const Matrix4x4 & b)
#endif
{ return a+b; }

/*
** matrix subtraction
*/
#ifdef OG
WWINLINE Matrix4 operator - (const Matrix4 & a, const Matrix4 & b)
#endif
#ifdef ZH
WWINLINE Matrix4x4 operator - (const Matrix4x4 & a, const Matrix4x4 & b)
#endif
{
#ifdef OG
	return Matrix4(
#endif
#ifdef ZH
	return Matrix4x4(
#endif
				a.Row[0] - b.Row[0],
				a.Row[1] - b.Row[1],
				a.Row[2] - b.Row[2],
				a.Row[3] - b.Row[3]
	);
}

#ifdef OG
WWINLINE Matrix4 Subtract(const Matrix4 & a, const Matrix4 & b)
#endif
#ifdef ZH
WWINLINE Matrix4x4 Subtract(const Matrix4x4 & a, const Matrix4x4 & b)
#endif
{ return a-b; }

/*
** matrix multiplication
*/
#ifdef OG
WWINLINE Matrix4 operator * (const Matrix4 & a, const Matrix4 & b)
#endif
#ifdef ZH
WWINLINE Matrix4x4 operator * (const Matrix4x4 & a, const Matrix4x4 & b)
#endif
{
	#define ROWCOL(i, j) a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j] + a[i][3]*b[3][j]
    
#ifdef OG
	return Matrix4(
#endif
#ifdef ZH
	return Matrix4x4(
#endif
		Vector4(ROWCOL(0,0), ROWCOL(0,1), ROWCOL(0,2), ROWCOL(0,3)),
		Vector4(ROWCOL(1,0), ROWCOL(1,1), ROWCOL(1,2), ROWCOL(1,3)),
		Vector4(ROWCOL(2,0), ROWCOL(2,1), ROWCOL(2,2), ROWCOL(2,3)),
		Vector4(ROWCOL(3,0), ROWCOL(3,1), ROWCOL(3,2), ROWCOL(3,3))
	);
	
	#undef ROWCOL
}

#ifdef OG
WWINLINE Matrix4 Multiply(const Matrix4 & a, const Matrix4 & b)
#endif
#ifdef ZH
WWINLINE Matrix4x4 Multiply(const Matrix4x4 & a, const Matrix4x4 & b)
#endif
{ return a*b; }

#ifdef OG
WWINLINE Matrix4 operator * (const Matrix4 & a, const Matrix3D & b)
#endif
#ifdef ZH
WWINLINE Matrix4x4 operator * (const Matrix4x4 & a, const Matrix3D & b)
#endif
{
	// This function hand coded to handle the last row of b as 0,0,0,1
	#define ROWCOL(i,j) a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j]
	#define ROWCOL_LAST(i,j) a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j] + a[i][3]
#ifdef OG
	return Matrix4(
#endif
#ifdef ZH
	return Matrix4x4(
#endif
		Vector4(ROWCOL(0,0), ROWCOL(0,1), ROWCOL(0,2), ROWCOL_LAST(0,3)),
		Vector4(ROWCOL(1,0), ROWCOL(1,1), ROWCOL(1,2), ROWCOL_LAST(1,3)),
		Vector4(ROWCOL(2,0), ROWCOL(2,1), ROWCOL(2,2), ROWCOL_LAST(2,3)),
		Vector4(ROWCOL(3,0), ROWCOL(3,1), ROWCOL(3,2), ROWCOL_LAST(3,3))
	);
	#undef ROWCOL
	#undef ROWCOL_LAST
}

#ifdef OG
WWINLINE Matrix4 operator * (const Matrix3D & a, const Matrix4 & b)
#endif
#ifdef ZH
WWINLINE Matrix4x4 operator * (const Matrix3D & a, const Matrix4x4 & b)
#endif
{
	// This function hand coded to handle the last row of a as 0,0,0,1
	#define ROWCOL(i,j) a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j] + a[i][3]*b[3][j]
#ifdef OG
	return Matrix4(
#endif
#ifdef ZH
	return Matrix4x4(
#endif
		Vector4(ROWCOL(0,0), ROWCOL(0,1), ROWCOL(0,2), ROWCOL(0,3)),
		Vector4(ROWCOL(1,0), ROWCOL(1,1), ROWCOL(1,2), ROWCOL(1,3)),
		Vector4(ROWCOL(2,0), ROWCOL(2,1), ROWCOL(2,2), ROWCOL(2,3)),
		Vector4(b[3][0], b[3][1], b[3][2], b[3][3])
	);
	#undef ROWCOL
}

/*
#ifdef OG
** Multiply a Matrix4 by a Vector3 (assumes w=1.0!!!). Yeilds a Vector4 result
#endif
#ifdef ZH
** Multiply a Matrix4x4 by a Vector3 (assumes w=1.0!!!). Yeilds a Vector4 result
#endif
*/
#ifdef OG
WWINLINE Vector4 operator * (const Matrix4 & a, const Vector3 & v) {
#endif
#ifdef ZH
WWINLINE Vector4 operator * (const Matrix4x4 & a, const Vector3 & v) {
#endif
	return Vector4(
		a[0][0] * v[0] + a[0][1] * v[1] + a[0][2] * v[2] + a[0][3] * 1.0f,
		a[1][0] * v[0] + a[1][1] * v[1] + a[1][2] * v[2] + a[1][3] * 1.0f,
		a[2][0] * v[0] + a[2][1] * v[1] + a[2][2] * v[2] + a[2][3] * 1.0f,
		a[3][0] * v[0] + a[3][1] * v[1] + a[3][2] * v[2] + a[3][3] * 1.0f
	);
}

/*
#ifdef OG
** Multiply a Matrix4 by a Vector4
#endif
#ifdef ZH
** Multiply a Matrix4x4 by a Vector4
#endif
*/
#ifdef OG
WWINLINE Vector4 operator * (const Matrix4 & a, const Vector4 & v) {
#endif
#ifdef ZH
WWINLINE Vector4 operator * (const Matrix4x4 & a, const Vector4 & v) {
#endif
	return Vector4(
		a[0][0] * v[0] + a[0][1] * v[1] + a[0][2] * v[2] + a[0][3] * v[3],
		a[1][0] * v[0] + a[1][1] * v[1] + a[1][2] * v[2] + a[1][3] * v[3],
		a[2][0] * v[0] + a[2][1] * v[1] + a[2][2] * v[2] + a[2][3] * v[3],
		a[3][0] * v[0] + a[3][1] * v[1] + a[3][2] * v[2] + a[3][3] * v[3]
	);
}

/*
#ifdef OG
** Multiply a Matrix4 by a Vector4
#endif
#ifdef ZH
** Multiply a Matrix4x4 by a Vector4
#endif
*/
#ifdef OG
WWINLINE void Matrix4::Transform_Vector(const Matrix4 & A,const Vector3 & in,Vector3 * out)
#endif
#ifdef ZH
WWINLINE void Matrix4x4::Transform_Vector(const Matrix4x4 & A,const Vector3 & in,Vector3 * out)
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

	out->X = (A[0][0] * v->X + A[0][1] * v->Y + A[0][2] * v->Z + A[0][3]);
	out->Y = (A[1][0] * v->X + A[1][1] * v->Y + A[1][2] * v->Z + A[1][3]);
	out->Z = (A[2][0] * v->X + A[2][1] * v->Y + A[2][2] * v->Z + A[2][3]);
}


#ifdef OG
WWINLINE void Matrix4::Transform_Vector(const Matrix4 & A,const Vector3 & in,Vector4 * out)
#endif
#ifdef ZH
WWINLINE void Matrix4x4::Transform_Vector(const Matrix4x4 & A,const Vector3 & in,Vector4 * out)
#endif
{
	out->X = (A[0][0] * in.X + A[0][1] * in.Y + A[0][2] * in.Z + A[0][3]);
	out->Y = (A[1][0] * in.X + A[1][1] * in.Y + A[1][2] * in.Z + A[1][3]);
	out->Z = (A[2][0] * in.X + A[2][1] * in.Y + A[2][2] * in.Z + A[2][3]);
	out->W = 1.0f;
}

#ifdef OG
WWINLINE void	Matrix4::Transform_Vector(const Matrix4 & A,const Vector4 & in,Vector4 * out)
#endif
#ifdef ZH
WWINLINE void	Matrix4x4::Transform_Vector(const Matrix4x4 & A,const Vector4 & in,Vector4 * out)
#endif
{
	Vector4 tmp;
	Vector4 * v;

	// check for aliased parameters
	if (out == &in) {
		tmp = in;
		v = &tmp;
	} else {
		v = (Vector4 *)&in;		// whats the right way to do this...
	}

	out->X = (A[0][0] * v->X + A[0][1] * v->Y + A[0][2] * v->Z + A[0][3] * v->W);
	out->Y = (A[1][0] * v->X + A[1][1] * v->Y + A[1][2] * v->Z + A[1][3] * v->W);
	out->Z = (A[2][0] * v->X + A[2][1] * v->Y + A[2][2] * v->Z + A[2][3] * v->W);
	out->W = (A[3][0] * v->X + A[3][1] * v->Y + A[3][2] * v->Z + A[3][3] * v->W);
}


#endif /*MATRIX4_H*/