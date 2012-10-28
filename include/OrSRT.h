// ******************************************************************************** //
// OrSQT.h																			//
// =======																			//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quite easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project is. You can do this by writing a comment at github.com/Jojendersie/.		//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //


#pragma once

namespace OrE {
namespace Math {

	class Matrix;

// ******************************************************************************** //
// Class for an scale-rotate-translate (in this order) transformation.
class SRT
{
	Vec3 m_vScale;
	Quaternion m_Quaternion;
	Vec3 m_vTranslation;
public:
	// ******************************************************************************** //
	// Constructors - copy is the standard one
	SRT() : m_vScale(1.0f), m_Quaternion(), m_vTranslation(0.0f)									{}
	SRT( const Matrix& _mTr )																		{ SetTransformation(_mTr); }
	SRT( const Vec3& _vScale, const Vec3& _vAxis, float _a, const Vec3& _vTr ) : m_vScale(_vScale), m_vTranslation(_vTr)	{ SetRotation( _vAxis, _a ); }

	// ******************************************************************************** //
	// The following functions all reset / exchange a part of the current transformation.
	// Set a uniform scaling or a different scaling along the coordinate-axis.
	void SetScale( float _s )							{ m_vScale.x = m_vScale.y = m_vScale.z = _s; }
	void SetScale( const Vec3& _s )						{ m_vScale = _s; }

	// Resets the quaternion for new rotation
	void SetRotation( const Vec3& _vAxis, float _a )	{m_Quaternion = Quaternion(_vAxis, _a);}
	void SetRotation( float _fYaw, float _fPitch, float _fRoll );

	// Reset translation vector
	void SetPosition( const Vec3& _vTr )				{ m_vTranslation = _vTr; }

	// Resets all transformations by decomposing an affine transformation matrix.
	// If the matrix is no affine transformation the result is undefined.
	void SetTransformation( const Matrix& _mTr );

	// ******************************************************************************** //
	// Functions to change values in respect to the current transformation.
	void Translate( const Vec3 _vTr )					{ m_vTranslation += _vTr; }
	void Rotate( const SRT& _Rot )
	{
		// Quaternion multiplication = successively rotation
		m_Quaternion *= _Rot.m_Quaternion;
	}
	void Rotate( const Quaternion& _Rot )				{ m_Quaternion *= _Rot; }
	void Rotate( float _fYaw, float _fPitch, float _fRoll )	{ m_Quaternion *= Quaternion( _fYaw, _fPitch, _fRoll ); }
	void Rotate( const Vec3& _vAxis, float _a )			{ m_Quaternion *= Quaternion( _vAxis, _a ); }
	void Scale( float _s )								{ m_vScale *= _s; }
	void Scale( const Vec3& _s )						{ m_vScale *= _s; }

	// ******************************************************************************** //
	// Functions to read back single transformations.
	const Vec3& GetScale()								{ return m_vScale; }
	const Vec3& GetPosition()							{ return m_vTranslation; }

	// The matrices are expensively computed by the call. Safe if needed more than one
	// time.
	Matrix GetTransformation();
	// The inverse is derived from analytical inverse of all transformations.
	// This should be more accurate than inverting the matrix.
	Matrix GetInverseTransformation();

	friend Vec3 operator * (const SRT& m, const Vec3& v);
	friend Vec3 operator * (const Vec3& v, const SRT& m);
};

// ******************************************************************************** //
// Lerp and Slerp both do the same for SRT transformations
SRT Lerp( const SRT& T1, const SRT& T2, const float t );//TODO
SRT Slerp( const SRT& T1, const SRT& T2, const float t );//TODO

// ******************************************************************************** //
// Transformation equal to Matrix*(Vec3,1).
// This is slower than matrix transformation -> for multiple transformations convert
// to matrix.
Vec3 operator * (const SRT& m, const Vec3& v);
// Transformation equal to (Vec3,1)*Matrix -> applies the SRT transformation to the
// vector.
Vec3 operator * (const Vec3& v, const SRT& m);

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //