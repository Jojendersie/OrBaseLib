// ******************************************************************************** //
// test_math.cpp																	//
// =============																	//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: Testing and benchmarking of all mathematical functions.					//
//																					//
// ******************************************************************************** //

#include "../include/OrBaseLib.h"
#include <iostream>

using namespace OrE::Math;

void test_hit();
void test_slerp();
void test_SRT();
void benchmark_hit( OrE::Utils::TimeQuerySlot& Time0 );
void benchmark_slerp( OrE::Utils::TimeQuerySlot& Time0 );
void benchmark_sgn( OrE::Utils::TimeQuerySlot& Time0 );

#define ms(x) (int(x*100000)/100.0)


// ******************************************************************************** //
void test_math()
{	
	OrE::Utils::TimeQuerySlot Time0;

	std::cout << "******** Start test OrE::Math ********\n";

	test_hit();
	test_slerp();
	test_SRT();


	std::cout << "\n******** Start benchmark OrE::Math ********\n";

	// Hit test and detection functions
	benchmark_hit( Time0 );

	// Transformations
	benchmark_slerp( Time0 );
	
	// Primitives - debug only otherwise they are removed be optimization
#ifdef _DEBUG
	benchmark_sgn( Time0 );
#endif
	// Sqrt, InvSqrt
/*	OrE::Utils::TimeQuery( Time0 );
	float fRes;
	double dRes;
	for( int i=0; i<10000; ++i )
		fRes = InvSqrt( 3.0f );
	double t1 = OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<10000; ++i )
		dRes = 1.0/sqrt( 3.0 );
	double t2 = OrE::Utils::TimeQuery( Time0 );
	std::cout << "\tInvSqrt is " << int(t2/t1*10.0)/10.0f << " times faster than 1.0/sqrt(): \n";*/

	std::cout << '\n';
}



// ******************************************************************************** //
// Hit tests
void test_hit()
{
	// Test all hit functions
	bool bSucc = true;
	Vec3 vFeedback;
	Ellipsoid E1(Vec3(0.0f), Vec3(1.0f));
	Ellipsoid E2(Vec3(2.0f, 0.0f, 0.0f), Vec3(1.1f, 0.5f, 2.0f));
	Ellipsoid E3(Vec3(2.0f, 0.1f, 0.0f), Vec3(1.0f, 1.0f, 2.0f));
	Ellipsoid E4(Vec3(0.1f, 3.0f, 0.0f), Vec3(0.8f, 3.5f, 0.5f));
	Sphere S1(Vec3(0.0f), 1.0f);
	Sphere S2(Vec3(1.0f, 1.0f, 0.0f), 0.35f);

	bSucc &= HitTest(E1, E2) == true;
	bSucc &= HitTest(E1, E3) == false;
	bSucc &= HitTest(E1, E4) == true;
	bSucc &= HitTest(E2, E4) == false;
	bSucc &= HitTest(S1, S2) == false;
	std::cout << (bSucc ? "\tHit testing: succeeded\n" : "\tHit testing: failed\n");
}

void benchmark_hit( OrE::Utils::TimeQuerySlot& Time0 )
{
	Vec3 vFeedback;
	Ellipsoid E1(Vec3(0.0f), Vec3(1.0f));
	Ellipsoid E2(Vec3(2.0f, 0.0f, 0.0f), Vec3(1.1f, 0.5f, 2.0f));
	Sphere S1(Vec3(0.0f), 1.0f);
	Sphere S2(Vec3(1.0f, 1.0f, 0.0f), 0.35f);

	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<100000; ++i )
		HitTest(S1, S2);
	std::cout << "\tTime for 100000 HitTest (Sphere-Sphere) calls: " << ms(OrE::Utils::TimeQuery( Time0 )) << " ms\n";
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<100000; ++i )
		HitTest(E1, E2);
	std::cout << "\tTime for 100000 HitTest (Ellipsoid-Ellipsoid) calls: " << ms(OrE::Utils::TimeQuery( Time0 )) << " ms\n";
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<100000; ++i )
		HitDetection(E1, E2, vFeedback);
	std::cout << "\tTime for 100000 HitDetection (Ellipsoid-Ellipsoid) calls: " << ms(OrE::Utils::TimeQuery( Time0 )) << " ms\n";
}



// ******************************************************************************** //
// Lerp, Slerp
void test_slerp()
{
	Vec2 v2Lerp1(1.0f, 0.0f);
	Vec2 v2Lerp2(0.0f, 1.0f);
	Vec2 v2Res = Slerp( v2Lerp1, v2Lerp2, 0.1f );
	Vec2 v2Res2 = Slerp( v2Lerp1, v2Lerp2, 0.9f );
	Vec2 v2Res3 = Slerp( v2Lerp1, v2Lerp2, 0.5f );
	Vec2 v2Res4 = Slerp( v2Res, v2Res2, 0.5f );
	std::cout << (( v2Res4 == v2Res3 ) ? "\tSlerp (Vec2): succeeded\n" : "\tSlerp: failed\n");

	Vec3 vLerp1(1.0f, 0.0f, 0.0f);
	Vec3 vLerp2(0.0f, 1.0f, 0.0f);
	Vec3 vRes = Slerp( vLerp1, vLerp2, 0.1f );
	Vec3 vRes2 = Slerp( vLerp1, vLerp2, 0.9f );
	Vec3 vRes3 = Slerp( vLerp1, vLerp2, 0.5f );
	Vec3 vRes4 = Slerp( vRes, vRes2, 0.5f );
	std::cout << (( vRes4 == vRes3 ) ? "\tSlerp (Vec3): succeeded\n" : "\tSlerp: failed\n");

	Vec4 v4Lerp1(1.0f, 0.0f, 0.0f, 0.0f);
	Vec4 v4Lerp2(0.0f, 1.0f, 0.0f, 0.0f);
	Vec4 v4Res = Slerp( v4Lerp1, v4Lerp2, 0.1f );
	Vec4 v4Res2 = Slerp( v4Lerp1, v4Lerp2, 0.9f );
	Vec4 v4Res3 = Slerp( v4Lerp1, v4Lerp2, 0.5f );
	Vec4 v4Res4 = Slerp( v4Res, v4Res2, 0.5f );
	std::cout << (( v4Res4 == v4Res3 ) ? "\tSlerp (Vec4): succeeded\n" : "\tSlerp: failed\n");
}

void benchmark_slerp( OrE::Utils::TimeQuerySlot& Time0 )
{
	// ********** Vec2 ***********
	OrE::Utils::TimeQuery( Time0 );
	Vec2 v2Lerp1(1.0f, 0.0f);
	Vec2 v2Lerp2(0.0f, 1.0f);
	for( int i=0; i<100000; ++i )
		v2Lerp1 = Slerp( v2Lerp1, v2Lerp2, 0.01f );
	std::cout << "\tTime for 100000 Slerp (Vec2) calls: " << ms(OrE::Utils::TimeQuery( Time0 )) << " ms\n";

	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<100000; ++i )
		v2Lerp1 = Lerp( v2Lerp1, v2Lerp2, 0.1f );
	std::cout << "\tTime for 100000 Lerp  (Vec2) calls: " << ms(OrE::Utils::TimeQuery( Time0 )) << " ms\n";

	// ********** Vec3 ***********
	OrE::Utils::TimeQuery( Time0 );
	Vec3 vLerp1(1.0f, 0.0f, 0.0f);
	Vec3 vLerp2(0.0f, 1.0f, 0.0f);
	for( int i=0; i<100000; ++i )
		vLerp1 = Slerp( vLerp1, vLerp2, 0.1f );
	std::cout << "\tTime for 100000 Slerp (Vec3) calls: " << ms(OrE::Utils::TimeQuery( Time0 )) << " ms\n";

	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<100000; ++i )
		vLerp1 = Lerp( vLerp1, vLerp2, 0.1f );
	std::cout << "\tTime for 100000 Lerp  (Vec3) calls: " << ms(OrE::Utils::TimeQuery( Time0 )) << " ms\n";

	// ********** Vec4 ***********
	OrE::Utils::TimeQuery( Time0 );
	Vec4 v4Lerp1(1.0f, 0.0f, 0.0f, 0.0f);
	Vec4 v4Lerp2(0.0f, 1.0f, 0.0f, 0.0f);
	for( int i=0; i<100000; ++i )
		v4Lerp1 = Slerp( v4Lerp1, v4Lerp2, 0.1f );
	std::cout << "\tTime for 100000 Slerp (Vec4) calls: " << ms(OrE::Utils::TimeQuery( Time0 )) << " ms\n";

	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<100000; ++i )
		v4Lerp1 = Lerp( v4Lerp1, v4Lerp2, 0.1f );
	std::cout << "\tTime for 100000 Lerp  (Vec4) calls: " << ms(OrE::Utils::TimeQuery( Time0 )) << " ms\n";
}



// ******************************************************************************** //
void test_SRT()
{
	bool bSucc = true;

	Vec3 vAxis = Vec3::Random();
	Vec3 vTranslation( 10.0f, 32.326f, 0.00001f );
	Vec3 vScale(0.1f, 2.0f, 3.5621f);
	Matrix mTr1 = MatrixScaling( vScale )
		* MatrixRotationAxis( vAxis, 1.456f )
		* MatrixTranslation( vTranslation );
	Matrix mTr2 = MatrixIdentity();
	SRT Srt1( mTr1 );
	SRT Srt1_2( vScale, vAxis, 1.456f, vTranslation );
	SRT Srt2( mTr2 );
	Vec3 v1 = Vec3::Random();
	Vec3 v2(1.0f, 0.0f, 0.0f);
	Vec3 v3(0.0f, 0.0f, 0.0f);
	Vec3 v4(-1.0f, 0.0f, -1.0f);

	// Test decomposition + composition
	bSucc &= Srt1.GetTransformation() == mTr1;
	bSucc &= Srt1_2.GetTransformation() == mTr1;
	bSucc &= Srt2.GetTransformation() == mTr2;

	// Test transformation
	bSucc &= (mTr1*v1) == (Srt1_2*v1);
	bSucc &= (v1*mTr1) == (v1*Srt1_2);
	bSucc &= (mTr1*v2) == (Srt1_2*v2);
	bSucc &= (v2*mTr1) == (v2*Srt1_2);
	bSucc &= (mTr1*v3) == (Srt1_2*v3);
	bSucc &= (v3*mTr1) == (v3*Srt1_2);
	bSucc &= (mTr1*v4) == (Srt1_2*v4);
	bSucc &= (v4*mTr1) == (v4*Srt1_2);

	bSucc &= MatrixIdentity() == ( Srt1_2.GetTransformation() * Srt1_2.GetInverseTransformation() );
	//Matrix mR = MatrixInvert( mTr1 ) - Srt1_2.GetInverseTransformation();

	std::cout << (bSucc ? "\tSRT testing: succeeded\n" : "\tSRT testing: failed\n");
}

// ******************************************************************************** //
void benchmark_sgn( OrE::Utils::TimeQuerySlot& Time0 )
{
	int x=1, y=-1, z=0;
	float fx=1, fy=-1, fz=0;
	double dx=1, dy=-1, dz=0;
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<100000; ++i )
	{
		x = Sgn( x ); y = Sgn( y ); x = Sgn( x ); y = Sgn( y ); z = Sgn( z );
		x = Sgn( x ); y = Sgn( y ); x = Sgn( x ); y = Sgn( y ); z = Sgn( z );
	}
	double t1 = OrE::Utils::TimeQuery( Time0 );

	for( int i=0; i<100000; ++i )
	{
		fx = Sgn( fx ); fy = Sgn( fy ); fx = Sgn( fx ); fy = Sgn( fy ); fz = Sgn( fz );
		fx = Sgn( fx ); fy = Sgn( fy ); fx = Sgn( fx ); fy = Sgn( fy ); fz = Sgn( fz );
	}
	double t2 = OrE::Utils::TimeQuery( Time0 );

	for( int i=0; i<100000; ++i )
	{
		dx = Sgn( dx ); dy = Sgn( dy ); dx = Sgn( dx ); dy = Sgn( dy ); dz = Sgn( dz );
		dx = Sgn( dx ); dy = Sgn( dy ); dx = Sgn( dx ); dy = Sgn( dy ); dz = Sgn( dz );
	}
	double t3 = OrE::Utils::TimeQuery( Time0 );
	std::cout << "\t1000000 Sgn    (int, float, double) calls: " << ms(t1) << " ms, " << ms(t2) << " ms, " << ms(t3) << " ms\n";

	x=1; y=-1; z=0;
	fx=1; fy=-1; fz=0;
	dx=1; dy=-1; dz=0;
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<100000; ++i )
	{
		x = Signum( x ); y = Signum( y ); x = Signum( x ); y = Signum( y ); z = Signum( z );
		x = Signum( x ); y = Signum( y ); x = Signum( x ); y = Signum( y ); z = Signum( z );
	}
	t1 = OrE::Utils::TimeQuery( Time0 );

	for( int i=0; i<100000; ++i )
	{
		fx = (float)Signum( fx ); fy = (float)Signum( fy ); fx = (float)Signum( fx ); fy = (float)Signum( fy ); fz = (float)Signum( fz );
		fx = (float)Signum( fx ); fy = (float)Signum( fy ); fx = (float)Signum( fx ); fy = (float)Signum( fy ); fz = (float)Signum( fz );
	}
	t2 = OrE::Utils::TimeQuery( Time0 );

	for( int i=0; i<100000; ++i )
	{
		dx = Signum( dx ); dy = Signum( dy ); dx = Signum( dx ); dy = Signum( dy ); dz = Signum( dz );
		dx = Signum( dx ); dy = Signum( dy ); dx = Signum( dx ); dy = Signum( dy ); dz = Signum( dz );
	}
	t3 = OrE::Utils::TimeQuery( Time0 );
	std::cout << "\t1000000 Signum (int, float, double) calls: " << ms(t1) << " ms, " << ms(t2) << " ms, " << ms(t3) << " ms\n";
}
