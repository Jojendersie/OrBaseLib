// ******************************************************************************** //
// OrAnalyticalGeometry.h															//
// ======================															//
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

	// ******************************************************************************** //
	// Primitive types of 3D geometry
	struct Sphere
	{
		Vec3 vMiddlePoint;
		float fRadius;

		Sphere( const Vec3& _vMiddlePoint, float _fRadius ) : vMiddlePoint( _vMiddlePoint ), fRadius( _fRadius ) {}
		Sphere( ) : vMiddlePoint(0.0f), fRadius(1.0f) {}
	};

	struct Ellipsoid
	{
		Vec3 vMiddlePoint;
		Vec3 vRadiiInvSq;

		Ellipsoid( const Vec3& _vMiddlePoint, const Vec3& _vRadii ) : vMiddlePoint( _vMiddlePoint ), vRadiiInvSq( Vec3(1.0f)/(_vRadii*_vRadii) ) {}
		Ellipsoid( ) : vMiddlePoint(0.0f), vRadiiInvSq(1.0f) {}
	};

	struct AABox
	{
		Vec3 vMin;
		Vec3 vMax;
	};

	struct Capsule
	{
		Vec3 vP1;
		Vec3 vP2;
		float fRadius;
	};

	// ******************************************************************************** //
	// Functions to determine the occurrence of a contact of two objects.
	// If you need to know where they hit use on of the HitDetection() functions. Do not
	// test with HitTest and call directly afterwards HitDetection, because both do nearly
	// the same (HitDetection includes the HitTest code).
	bool HitTest( Sphere, Sphere );
	bool HitTest( Ellipsoid, Ellipsoid );

	// ******************************************************************************** //
	// Function to determine the location and intensity of a contact of two objects.
	// Returns whether there is a hit or not. If the objects hits the feedback vector is
	// overwritten. Otherwise the vector could be overwritten with a point somewhere between
	// the objects (but it as not to be).
	bool HitDetection( Ellipsoid, Ellipsoid, Vec3& _vFeedbackLocation );

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //