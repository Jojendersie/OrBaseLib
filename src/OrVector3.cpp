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
// project is. You can do this by writing a comment at github.com/Jojendersie/.		//
//																					//
// For details on this project see: Readme.txt										//
// ******************************************************************************** //

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrRandom.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrMatrix.h"

using namespace OrE::Algorithm;
using namespace OrE::Math;

// ******************************************************************************** //
// Aus Abhängigkeitsgründen separieren
Vec3 OrE::Math::Vec3::Random()
{
	return Vec3(Rand()*2.0f-1.0f, Rand()*2.0f-1.0f, Rand()*2.0f-1.0f).NormalizeEx();
}

// ******************************************************************************** //
IVec3 OrE::Math::IVec3::Random()
{
	return IVec3( int(RandU()), int(RandU()), int(RandU()) ).Normalize( 1000 );
}

// ******************************************************************************** //
// 3D-Positionsvektor transformieren
/*Vec3 OrE::Math::Vec3::TransformCoords(const Matrix& m,
									  float& fOutW)
{
	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	fOutW = x * m.m14 + y * m.m24 + z * m.m34 + m.m44;
	double dInvW = 1.0/fOutW;
	// Vektor mit Matrix multiplizieren
	return Vec3(float( dInvW * (x * m.m11 + y * m.m21 + z * m.m31 + m.m41) ),
		        float( dInvW * (x * m.m12 + y * m.m22 + z * m.m32 + m.m42) ),
				float( dInvW * (x * m.m13 + y * m.m23 + z * m.m33 + m.m43) ));
}

Vec3 OrE::Math::Vec3::TransformCoords(const Vec3& v,
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
const Vec3& OrE::Math::Vec3::TransformCoords(const Matrix& m)
{
	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	const double w = 1.0/(x * m.m14 + y * m.m24 + z * m.m34 + m.m44);

	// Vektor mit Matrix multiplizieren
	x = float( w * (x * m.m11 + y * m.m21 + z * m.m31 + m.m41) );
	y = float( w * (x * m.m12 + y * m.m22 + z * m.m32 + m.m42) );
	z = float( w * (x * m.m13 + y * m.m23 + z * m.m33 + m.m43) );

	return *this;
}

Vec3 OrE::Math::Vec3::TransformCoords(const Vec3& v,
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
}*/

// ******************************************************************************** //
// 3D-Richtungsvektor transformieren
Vec3 OrE::Math::Vec3::TransformDirection(const Matrix& m) const
{
	// Vektor mit Matrix multiplizieren
	return Vec3(x * m.m11 + y * m.m21 + z * m.m31,// + m.m41,
				x * m.m12 + y * m.m22 + z * m.m32,// + m.m42,
				x * m.m13 + y * m.m23 + z * m.m33);// + m.m43);

	// Vierte Koordinate (w) berechnen entfällt da es sich um Richtungsvektoren handelt

	return *this;
}

/*Vec3 OrE::Math::Vec3::TransformDirection(const Vec3& v,
									     const Matrix& m)
{
	// Vektor mit Matrix multiplizieren
	return Vec3(v.x * m.m11 + v.y * m.m21 + v.z * m.m31,// + m.m41,
				v.x * m.m12 + v.y * m.m22 + v.z * m.m32,// + m.m42,
				v.x * m.m13 + v.y * m.m23 + v.z * m.m33);// + m.m43);

	// Vierte Koordinate (w) berechnen entfällt da es sich um Richtungsvektoren handelt
}*/

// ******************************************************************************** //
// Rotate a vector around some axis. This transformation uses quaternion
// math. Use this if you want to rotate onle one vector. For the rotation
// of multiple vectors a matrix is more efficient!
Vec3 OrE::Math::Vec3::Rotate(const Vec3& vAxis, float fAngle) const
{
	// Calculate quaternion Errors in Code??
	/*double x = cos(fAngle*0.5);
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

	return Vec3(float(x * (a00 + a11 - a22 - a33) + 2.0 * ((a12 - a03) * y + (a13 + a02) * z)),
				float(y * (a00 - a11 + a22 - a33) + 2.0 * ((a12 + a03) * x + (a23 - a01) * z)),
				float(z * (a00 - a11 - a22 + a33) + 2.0 * ((a13 - a02) * x + (a23 + a01) * y)));*/

	// Direct Matrix variant (faster and not wrong like the one above)
	const double dSin = sin(double(-fAngle));
	const double dCos = cos(double(-fAngle));
	const double dOneMinusCos = 1.0 - dCos;
	const double dAxSin = vAxis.x * dSin;
	const double dAySin = vAxis.y * dSin;
	const double dAzSin = vAxis.z * dSin;

	// Create the matrix:
	const double c12 = (vAxis.x * vAxis.y) * dOneMinusCos;
	const double c13 = (vAxis.x * vAxis.z) * dOneMinusCos;
	const double c23 = (vAxis.y * vAxis.z) * dOneMinusCos;
	double m11 = (vAxis.x * vAxis.x) * dOneMinusCos + dCos;	double m12 = c12 - dAzSin;								double m13 = c13 + dAySin;
	double m21 = c12 + dAzSin;								double m22 = (vAxis.y * vAxis.y) * dOneMinusCos + dCos;	double m23 = c23 - dAxSin;
	double m31 = c13 - dAySin;								double m32 = c23 + dAxSin;								double m33 = (vAxis.z * vAxis.z) * dOneMinusCos + dCos;

	return Vec3(float(x * m11 + y * m21 + z * m31),
				float(x * m12 + y * m22 + z * m32),
				float(x * m13 + y * m23 + z * m33));
/*	return Vec3(float(x * m11 + y * m12 + z * m13),
				float(x * m21 + y * m22 + z * m23),
				float(x * m31 + y * m32 + z * m33));*/
}

/*Vec3 OrE::Math::Vec3::Rotate(const Vec3& v, const Vec3& vAxis, float fAngle)
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
}*/

// *************************************EOF**************************************** //