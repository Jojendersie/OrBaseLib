// ******************************************************************************** //
// OrMultimap.cpp																	//
// ==============																	//
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

#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrHash.h"
#include "..\include\OrMultimap.h"

// ******************************************************************************** //
// Constructor creates an empty map of a choosen size.
OrE::ADT::Multimap::Multimap( int _iSize ) :
	m_GroupMap( 16, HashMapMode(HashMapMode::HM_NO_RESIZE | HashMapMode::HM_USE_STRING_MODE) ),
	m_ObjectMap( _iSize, HashMapMode::HM_PREFER_PERFORMANCE )
{
}
