// ******************************************************************************** //
// OrVec3.cpp																	//
// =============																	//
// This file is part of the OrBaseLib.												//
//																					//
// Author: Johannes Jendersie														//
//																					//
// Here is a quiete easy licensing as open source:									//
// http://creativecommons.org/licenses/by/3.0/										//
// If you use parts of this project, please let me know what the purpose of your	//
// project. You can do this by a comment at	https://github.com/Jojendersie/.		//
// Futhermore you have to state this project as a source of your project.			//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrRandom.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrMatrix.h"

using namespace OrE::Algorithm;
using namespace OrE::Math;

// ******************************************************************************** //
// Aus Abhängigkeitsgründen separieren
Vec3 OrE::Math::Vec3Random()
{
	return Vec3NormalizeEx(Vec3(Rand()*2.0f-1.0f, Rand()*2.0f-1.0f, Rand()*2.0f-1.0f));
}

// ******************************************************************************** //
// 3D-Positionsvektor transformieren
Vec3 OrE::Math::Vec3TransformCoords(const Vec3& v,
									const Matrix& m,
									float& fOutW)
{
	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	fOutW = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44;
	double dInvW = 1.0/fOutW;
	// Vektor mit Matrix multiplizieren
	return Vec3(float( dInvW * (v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41) ),
		        float( dInvW * (v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42) ),
				float( dInvW * (v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43) ));
}

// ******************************************************************************** //
// 3D-Positionsvektor transformieren
Vec3 OrE::Math::Vec3TransformCoords(const Vec3& v,
									const Matrix& m)
{
	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	const double w = 1.0/(v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44);

	// Vektor mit Matrix multiplizieren
	return Vec3(float( w * (v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41) ),
		        float( w * (v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42) ),
				float( w * (v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43) ));

	//return vResult / w;
}

// ******************************************************************************** //
// 3D-Richtungsvektor transformieren
Vec3 OrE::Math::Vec3TransformDirection(const Vec3& v,
									   const Matrix& m)
{
	// Vektor mit Matrix multiplizieren
	return Vec3(v.x * m.m11 + v.y * m.m21 + v.z * m.m31,// + m.m41,
				v.x * m.m12 + v.y * m.m22 + v.z * m.m32,// + m.m42,
				v.x * m.m13 + v.y * m.m23 + v.z * m.m33);// + m.m43);

	// Vierte Koordinate (w) berechnen entfällt da es sich um Richtungsvektoren handelt

	//return vResult;
}

// ******************************************************************************** //
// Rotate a vector around some axis. This transformation uses quaternion
// math. Use this if you want to rotate onle one vector. For the rotation
// of multiple vectors a matrix is more efficient!
Vec3 OrE::Math::Vec3Rotate(const Vec3& v, const Vec3& vAxis, float fAngle)
{
	// Calculate quaternion
	double x = cos(fAngle*0.5);
	double i = sin(fAngle*0.5);
	double j = vAxis.y * i;
	double k = vAxis.z * i;
	i *= vAxis.x;

	const double a00 = x * x;
	const double a01 = x * i;
	const double a02 = x * j;
	const double a03 = x * k;
	const double a11 = i * i;
	const double a12 = i * j;
	const double a13 = i * k;
	const double a22 = j * j;
	const double a23 = j * k;
	const double a33 = k * k;

	return Vec3(float(v.x * (a00 + a11 - a22 - a33) + 2.0 * ((a12 - a03) * v.y + (a13 + a02) * v.z)),
				float(v.y * (a00 - a11 + a22 - a33) + 2.0 * ((a12 + a03) * v.x + (a23 - a01) * v.z)),
				float(v.z * (a00 - a11 - a22 + a33) + 2.0 * ((a13 - a02) * v.x + (a23 + a01) * v.y)));
}

// *************************************EOF**************************************** //