// ******************************************************************************** //
// OrSQT.cpp																		//
// =========																		//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project is. You can do this by writing a comment at github.com/Jojendersie/.		//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrVector4.h"
#include "..\Include\OrMatrix.h"
#include "..\Include\OrSRT.h"



// ******************************************************************************** //
// Resets the quaternion for new rotation
// Inperformant - but grants that the rotation is the same as for matrices and it is
// the easyest implementation.
void OrE::Math::SRT::SetRotation( float _fYaw, float _fPitch, float _fRoll )
{
	// Build matrix and do transformation to quaternion
	m_Quaternion = Quaternion( MatrixRotation( _fYaw, _fPitch, _fRoll ) );

	// Safe original translation and scale
/*	Vec3 vScale = m_vScale;
	Vec3 vTranslation = m_vTranslation;

	// Create quaternion from rotation matrix.
	// This resets scale to one and translation to 0 -> safed values.
	SetTransformation( MatrixRotation( _fYaw, _fPitch, _fRoll ) );

	m_vScale = vScale;
	m_vTranslation = vTranslation;*/
}

// ******************************************************************************** //
// Resets all transformations by decomposing an affine transformation matrix.
void OrE::Math::SRT::SetTransformation( const Matrix& _mTr )
{
	// Translation is easy - read out
	m_vTranslation = Vec3( _mTr.m41, _mTr.m42, _mTr.m43 );

	Matrix M(_mTr.m11, _mTr.m12, _mTr.m13, 0.0f,
			 _mTr.m21, _mTr.m22, _mTr.m23, 0.0f,
			 _mTr.m31, _mTr.m32, _mTr.m33, 0.0f,
			 0.0f,     0.0f,     0.0f,     1.0f);

	// The scale is the length of the vector, which contributes to the coordinate
	// during a transformation.
	m_vScale.x = ((Vec3*)(M.m1))->Length();
	m_vScale.y = ((Vec3*)(M.m2))->Length();
	m_vScale.z = ((Vec3*)(M.m3))->Length();

	// After normalization M should by an orthonormal basis.
	// If not _mTr was no affine transformation. This is not tested here!
	(*(Vec3*)(M.m1)) /= m_vScale.x;
	(*(Vec3*)(M.m2)) /= m_vScale.y;
	(*(Vec3*)(M.m3)) /= m_vScale.z;

	// Build Quaternion from rotation matrix
	// Src: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	float fTrace = M.m11 + M.m22 + M.m33;
	if( fTrace > 0 )
	{
		float s = 0.5f / sqrtf( fTrace+ 1.0f );
		m_Quaternion.i = ( M.m32 - M.m23 ) * s;
		m_Quaternion.j = ( M.m13 - M.m31 ) * s;
		m_Quaternion.k = ( M.m21 - M.m12 ) * s;
		m_Quaternion.r = 0.25f / s;
	} else {
		if( M.m11 > M.m22 && M.m11 > M.m33 )
		{
			float s = 2.0f * sqrtf( 1.0f + M.m11 - M.m22 - M.m33 );
			m_Quaternion.i = 0.25f * s;
			m_Quaternion.j = ( M.m12 + M.m21 ) / s;
			m_Quaternion.k = ( M.m13 + M.m31 ) / s;
			m_Quaternion.r = ( M.m32 - M.m23 ) / s;
		} else if( M.m22 > M.m33 )
		{
			float s = 2.0f * sqrtf( 1.0f + M.m22 - M.m11 - M.m33 );
			m_Quaternion.i = ( M.m12 + M.m21 ) / s;
			m_Quaternion.j = 0.25f * s;
			m_Quaternion.k = ( M.m23 + M.m32 ) / s;
			m_Quaternion.r = ( M.m13 - M.m31 ) / s;
		} else {
			float s = 2.0f * sqrtf( 1.0f + M.m33 - M.m11 - M.m22 );
			m_Quaternion.i = ( M.m13 + M.m31 ) / s;
			m_Quaternion.j = ( M.m23 + M.m32 ) / s;
			m_Quaternion.k = 0.25f * s;
			m_Quaternion.r = ( M.m21 - M.m12 ) / s;
		}
	}
}

// ******************************************************************************** //
OrE::Math::Matrix OrE::Math::SRT::GetTransformation()
{
	// Rotation composition from quaternion (remaining rest direct in matrix)
	float f2i  = 2.0f * m_Quaternion.i;
	float f2j  = 2.0f * m_Quaternion.j;
	float f2k  = 2.0f * m_Quaternion.k;
    float f2ri = f2i  * m_Quaternion.r;
    float f2rj = f2j  * m_Quaternion.r;
    float f2rk = f2k  * m_Quaternion.r;
    float f2ii = f2i  * m_Quaternion.i;
    float f2ij = f2j  * m_Quaternion.i;
    float f2ik = f2k  * m_Quaternion.i;
    float f2jj = f2j  * m_Quaternion.j;
    float f2jk = f2k  * m_Quaternion.j;
    float f2kk = f2k  * m_Quaternion.k;

	// Composed matrix: Scale * Rotation * Translation
	return Matrix(
		m_vScale.x*(1.0f - ( f2jj + f2kk )), m_vScale.x*(f2ij - f2rk),            m_vScale.x*(f2ik + f2rj),            0.0f,
		m_vScale.y*(f2ij + f2rk),            m_vScale.y*(1.0f - ( f2ii + f2kk )), m_vScale.y*(f2jk - f2ri),            0.0f,
		m_vScale.z*(f2ik - f2rj),            m_vScale.z*(f2jk + f2ri),            m_vScale.z*(1.0f - ( f2ii + f2jj )), 0.0f,
		m_vTranslation.x,                    m_vTranslation.y,                    m_vTranslation.z,                    1.0f
	);
}

// ******************************************************************************** //
// The inverse is deriverd from analytical inersa of all transformations.
// This should be more accurate than inverting the matrix.
OrE::Math::Matrix OrE::Math::SRT::GetInverseTransformation()
{
	// Rotation composition from Quaternion (remaining rest direct in matrix)
	// To invert the rotation conjugate the quaternion.
	float f2i  = 2.0f * - m_Quaternion.i;
	float f2j  = 2.0f * - m_Quaternion.j;
	float f2k  = 2.0f * - m_Quaternion.k;
    float f2ri = f2i  *   m_Quaternion.r;
    float f2rj = f2j  *   m_Quaternion.r;
    float f2rk = f2k  *   m_Quaternion.r;
    float f2ii = f2i  * - m_Quaternion.i;
    float f2ij = f2j  * - m_Quaternion.i;
    float f2ik = f2k  * - m_Quaternion.i;
    float f2jj = f2j  * - m_Quaternion.j;
    float f2jk = f2k  * - m_Quaternion.j;
    float f2kk = f2k  * - m_Quaternion.k;
	float m11 = (1.0f - ( f2jj + f2kk )),  m12 = (f2ij - f2rk),             m13 = (f2ik + f2rj);
	float m21 = (f2ij + f2rk),             m22 = (1.0f - ( f2ii + f2kk )),  m23 = (f2jk - f2ri);
	float m31 = (f2ik - f2rj),             m32 = (f2jk + f2ri),             m33 = (1.0f - ( f2ii + f2jj ));

	// Composed matrix: Translation^-1 * Rotation^-1 * Scale^-1
	return Matrix(
		m11/m_vScale.x, m12/m_vScale.y, m13/m_vScale.z, 0.0f,
		m21/m_vScale.x, m22/m_vScale.y, m23/m_vScale.z, 0.0f,
		m31/m_vScale.x, m32/m_vScale.y, m33/m_vScale.z, 0.0f,
		(-m_vTranslation.x*m11 -m_vTranslation.y*m21 -m_vTranslation.z*m31)/m_vScale.x, (-m_vTranslation.x*m12 -m_vTranslation.y*m22 -m_vTranslation.z*m32)/m_vScale.y, (-m_vTranslation.x*m13 -m_vTranslation.y*m23 -m_vTranslation.z*m33)/m_vScale.z, 1.0f
	);
}

// ******************************************************************************** //
// Transformation equal to Matrix*(Vec3,1).
OrE::Math::Vec3 OrE::Math::operator * (const OrE::Math::SRT& m, const OrE::Math::Vec3& v)
{
	// Rotation-matrix composition from Quaternion
	float f2i  = 2.0f * m.m_Quaternion.i;
	float f2j  = 2.0f * m.m_Quaternion.j;
	float f2k  = 2.0f * m.m_Quaternion.k;
    float f2ri = f2i  * m.m_Quaternion.r;
    float f2rj = f2j  * m.m_Quaternion.r;
    float f2rk = f2k  * m.m_Quaternion.r;
    float f2ii = f2i  * m.m_Quaternion.i;
    float f2ij = f2j  * m.m_Quaternion.i;
    float f2ik = f2k  * m.m_Quaternion.i;
    float f2jj = f2j  * m.m_Quaternion.j;
    float f2jk = f2k  * m.m_Quaternion.j;
    float f2kk = f2k  * m.m_Quaternion.k;

	// Uniform coordinates in 4D -> division through w
	const float w = 1.0f/(v.x * m.m_vTranslation.x + v.y * m.m_vTranslation.y + v.z * m.m_vTranslation.z + 1.0f);

	return OrE::Math::Vec3(
		w * m.m_vScale.x * ( (1.0f - (f2jj + f2kk))*v.x + (f2ij - f2rk)*v.y + (f2ik + f2rj)*v.z ),
		w * m.m_vScale.y * ( (f2ij + f2rk)*v.x + (1.0f - (f2ii + f2kk))*v.y + (f2jk - f2ri)*v.z ),
		w * m.m_vScale.z * ( (f2ik - f2rj)*v.x + (f2jk + f2ri)*v.y + (1.0f - (f2ii + f2jj))*v.z )
		);
}

// ******************************************************************************** //
// Transformation equal to (Vec3,1)*Matrix -> applies the srt transformation to the
// vector.
OrE::Math::Vec3 OrE::Math::operator * (const OrE::Math::Vec3& v, const OrE::Math::SRT& m)
{
	// Rotation-matrix composition from Quaternion
	float f2i  = 2.0f * m.m_Quaternion.i;
	float f2j  = 2.0f * m.m_Quaternion.j;
	float f2k  = 2.0f * m.m_Quaternion.k;
    float f2ri = f2i  * m.m_Quaternion.r;
    float f2rj = f2j  * m.m_Quaternion.r;
    float f2rk = f2k  * m.m_Quaternion.r;
    float f2ii = f2i  * m.m_Quaternion.i;
    float f2ij = f2j  * m.m_Quaternion.i;
    float f2ik = f2k  * m.m_Quaternion.i;
    float f2jj = f2j  * m.m_Quaternion.j;
    float f2jk = f2k  * m.m_Quaternion.j;
    float f2kk = f2k  * m.m_Quaternion.k;

	// Uniform coordinates in 4D -> division through w, but w is always 1.
	// This is because the last column of the composed matrix is (0,0,0,1)'.

	float fX = v.x * m.m_vScale.x;
	float fY = v.y * m.m_vScale.y;
	float fZ = v.z * m.m_vScale.z;
	return OrE::Math::Vec3(
		fX * (1.0f - ( f2jj + f2kk )) + fY * (f2ij + f2rk) + fZ * (f2ik - f2rj) + m.m_vTranslation.x,
		fX * (f2ij - f2rk) + fY * (1.0f - ( f2ii + f2kk )) + fZ * (f2jk + f2ri) + m.m_vTranslation.y,
		fX * (f2ik + f2rj) + fY * (f2jk - f2ri) + fZ * (1.0f - ( f2ii + f2jj )) + m.m_vTranslation.z
		);
}

// *************************************EOF**************************************** //