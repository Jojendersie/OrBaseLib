// ******************************************************************************** //
// OrDebug.h																		//
// =========																		//
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
//									CONTENTS										//
// Some useful functions to get debug informations.		 							//
// If you include this header you get an garbage collector for each new in your		//
// code. This collector does only tests for memory-leaks at program end. You still	//
// have to write clean code and release your resources. This is just a tool to help	//
// you with it.																		//
// ******************************************************************************** //

#pragma once
#ifdef _DEBUG

// ******************************************************************************** //
// CUSTOMIZATION of the garbage collector.

// Use the following definition to write memory leaks into a file.
// You can change the name of the file how you want.
// NOT SUPPORTED YET see https://github.com/Jojendersie/ (OrDebug.xxx) for updates.
//#define OR_GC_MEMLEAK_FILE	"memleaks.txt"

// Use OR_GC_DEBUGOUT to enable output to the Visual Studio Debug console. If you use it
// with an other compiler compilation will fail.
#define OR_GC_DEBUGOUT

// Break at the end, if memory leaks are detected. The breakpoint is triggered ones per
// leak.
#define OR_GC_BREAK



// ******************************************************************************** //
// Operator overloading.
void* operator new(size_t sz, const char* pcFile, int iLine);

// This deletes are even used, if memory was not allocated with our
// new operator. The drawback is a little performance loss.
void operator delete(void* m);
void operator delete[](void* m);
void operator delete(void* m, const char* pcFile, int iLine);
void operator delete[](void* m, const char* pcFile, int iLine);

// ******************************************************************************** //
// This line enables the garbage collector. The implementation of the new(file, line)
// is in the OrDebug.cpp.
// THIS LINE CAN CAUSES ERRORS IN STDLIB.
// You have two options: don't use it and write new(__FILE__, __LINE__) manually
// everywhere or rename the macro.
// OR include this header as the very last one (Or at least after the std-headers).
#define new new(__FILE__, __LINE__)






// ******************************************************************************** //

// Complex, slow assertion to check pointer validity.
// You can use the function IsPointerInvalid to handle bad pointers your own way.
#define AssertPointerValidity(p) if( IsPointerInvalid(p) ) __debugbreak()


#else	// _DEBUG

#define AssertPointerValidity(p)

#endif // _DEBUG

// ******************************************************************************** //
bool IsPointerInvalid(const void* _Pointer);


// *************************************EOF**************************************** //