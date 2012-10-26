// ******************************************************************************** //
// OrAnalyticalGeometry.cpp															//
// ========================															//
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

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrPlane.h"
#include "..\include\OrAnalyticalGeometry.h"

// ******************************************************************************** //
// Implementations of hit test functions

bool OrE::Math::HitTest( Sphere s1, Sphere s2 )
{
	// The distance between the middlepoints has to be bigger than the sum of the radii.
	return (s1.vMiddlePoint-s2.vMiddlePoint).LengthSq() <= Sqr(s1.fRadius+s2.fRadius);
}

bool OrE::Math::HitTest( Ellipsoid e1, Ellipsoid e2 )
{
	// Absolute squared distance vector from e2 to e1
	Vec3 vDist = e1.vMiddlePoint - e2.vMiddlePoint;
	vDist *= vDist;
	// Project ray to both ellipsoid surfaces. All what matters is,
	// the the sum of the distances to surface is smaller than the length of
	// the distance vector.
	// Vec3 vRSum = vDist * ( InvSqrt( vDist.Dot( vDist * e1.vRadiiInvSq ) ) + InvSqrt( vDist.Dot( vDist * e2.vRadiiInvSq ) ) );
	// return vRSum.LengthSq() >= vDist.LengthSq();
	return InvSqrt( vDist.Dot( e1.vRadiiInvSq ) ) + InvSqrt( vDist.Dot( e2.vRadiiInvSq ) ) >= 1.0f;
}

// ******************************************************************************** //
// Implementations of hit detection functions

bool OrE::Math::HitDetection( Ellipsoid e1, Ellipsoid e2, Vec3& _vFeedbackLocation )
{
	// Absolute squared distance vector from e2 to e1
	Vec3 vDist = e1.vMiddlePoint - e2.vMiddlePoint;
	Vec3 vDistSq = vDist * vDist;
	// Determine the point in the middle between both surfaces.
	// Point on surface from e1, which is nerest to the center of e2 is p1 = r1 * -vDist + e1.m.
	// Point on surface from e2, which is nerest to the center of e1 is p2 = r2 * vDist + e2.m.
	// The point between them is (p1+p2)/2 = ((r2-r1) * vDist + e1.m+e2.m)/2
	float r1 = InvSqrt( vDistSq.Dot( e1.vRadiiInvSq ) );
	float r2 = InvSqrt( vDistSq.Dot( e2.vRadiiInvSq ) );
	_vFeedbackLocation = 0.5f*((r2-r1) * vDist + e1.vMiddlePoint + e2.vMiddlePoint);

	// Same as in HitTest
	return r1+r2 >= 1.0f;
}