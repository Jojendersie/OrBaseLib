/*************************************************************************

	OrVector2.cpp
	=============
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Rechnen mit 2D-Vektoren

*************************************************************************/

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrRandom.h"

// ******************************************************************** //
inline OrVector2 OrVec2Random()
{
	return OrVec2NormalizeEx(OrVector2(OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f));
}

// ******************************************************************** //
// Rotieren eines Punktes  in 2D-Raum
OrVector2 OrVector2Rotate(const OrVector2& _v,
						  const float _fAlpha)
{
	float fCos = OrCos(_fAlpha);
	float fSin = OrSin(_fAlpha);
	return OrVector2(_v.x*fCos-_v.y*fSin, _v.x*fSin+_v.y*fCos);
}
// ******************************************************************** //