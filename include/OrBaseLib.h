// ******************************************************************************** //
// OrBaseLib.h																		//
// ===========																		//
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

#ifdef _DEBUG
	#pragma comment(lib, "OrBaseLib32D.lib")
#else
	#pragma comment(lib, "OrBaseLib32.lib")
#endif

#include <mutex>

// Raw Types
#include "OrTypeDef.h"
#include "OrAssert.h"

// Math
#include "OrFastMath.h"
#include "OrVector2.h"
#include "OrVector3.h"
#include "OrVector4.h"
#include "OrSRT.h"
#include "OrPlane.h"
#include "OrMatrix.h"
#include "OrComplexNumber.h"
#include "OrAnalyticalGeometry.h"

// ADT
#include "OrADTObjects.h"
#include "OrBinaryTree.h"
#include "OrAVLTree.h"
#include "OrSplayTree.h"
#include "OrTrie.h"
#include "OrBuffer.h"
#include "OrHeap.h"
#include "OrHash.h"
#include "OrList.h"
#include "OrSetBuilder.h"
#include "OrMultiMap.h"
#include "OrGraph.h"

// Algorithms
#include "OrHuffman.h"
#include "OrRandom.h"
#include "OrUtils.h"
#include "OrFFT.h"

// String
#include "string/OrTokenizer.h"