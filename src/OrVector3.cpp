/*************************************************************************

	OrVector3.cpp
	=============
	Diese Datei ist Teil der Orkteck-Engine.

	Zweck:
	Rechnen mit 3D-Vektoren
	
*************************************************************************/

// Die Operatoren und Funktionen sind direkt per Inline-Funktion implementiert.

#include "..\Include\OrTypeDef.h"
#include "..\Include\OrFastMath.h"
#include "..\Include\OrRandom.h"
#include "..\Include\OrVector3.h"

// ******************************************************************** //
// Aus Abhängigkeitsgründen separieren
OrVector3 OrVec3Random()
{
	return OrVec3NormalizeEx(OrVector3(OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f, OrRand()*2.0f-1.0f));
}

// ******************************************************************** //