/*************************************************************************

	OrVector3.cpp
	=============
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Rechnen mit 4D-Vektoren
	
	Statistik:
	In Arbeit: Nein
	Bugs: Keine
	Speicherlecks: Keine
	Unvertig: ---

*************************************************************************/

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrRandom.h"
#include "..\Include\OrVector2.h"
#include "..\Include\OrVector3.h"
#include "..\Include\OrVector4.h"

// Die Operatoren und Funktionen sind direkt per Inline-Funktion implementiert.

// ******************************************************************** //
OrVector4 OrVec4Random()
{
	return OrVec4NormalizeEx(OrVector4(OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f));
}

// ******************************************************************** //