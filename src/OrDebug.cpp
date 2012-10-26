// ******************************************************************************** //
// OrDebug.cpp																		//
// ===========									s									//
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


#include <csignal>
#include <string>
#include "..\include\OrDebug.h"

#pragma warning(disable: 4996)

// New callback used by hardware interrupt for access violation
void __AccessVoilationSignalHandler(int signal) { throw "Access Violation!"; }

// Function testing a pointer
bool IsPointerInvalid(const void* _Pointer)
{
	bool bRet = false;
	// Set callback
	signal( SIGSEGV, __AccessVoilationSignalHandler );
	try {
		// try to get the exception
		char c=*(const char*)_Pointer;
	} catch(...) {
		// Yes this is an invalid pointer
		bRet = true;
	}

	// switch off this exceptions to get the better break, if a
	// real access violation occurs.
	signal( SIGSEGV, nullptr );

	return bRet;
}

#ifdef _DEBUG

#include "..\include\OrTypeDef.h"
#include "..\include\OrADTObjects.h"
#include "..\include\OrHash.h"

// ******************************************************************************** //
// Implementation of the garbage collector.
#ifdef OR_GC_DEBUGOUT
#include <Windows.h>
#endif

// ******************************************************************************** //
// PART I the Garbage
// The garbage objects saves allocation location and the size of the memory.
// This Object has an design size of 32 bytes (don't really no if that matters).
// You can change the acFileName if you want. Per default your filenames (without paths)
// should not longer than 23 bytes (truncated). If you change the constant change it
// in the sprintf as well.
struct Garbage
{
	int iSize;
	int iLine;
	char acFileName[24];

	Garbage(int _iSize, const char* _pcFile, int _iLine) : iSize( _iSize ), iLine( _iLine )
	{
		// Copy the last part of the filename (without dictionary)
		int iLen = strlen(_pcFile);
		for(int i=iLen-1; i>=0; --i)
			if(_pcFile[i] == '\\' || _pcFile[i] == '/')
			{
				sprintf(acFileName, "%.23s", &_pcFile[i+1], iLine);
				break;
			}
	}
};

// Each memory is identified by its address (obviously)
//typedef std::pair<void*, Garbage> PGarbage;

// PART II the collector
// Store all garbage objects in the collector and remove them on delete.
// The destructor of this class does the leak testing.
class Collector
{
public:
	// Not that clean -> should be private, but this knowledge of the class.. will
	// never leave the borders of this file!
	//std::unordered_map<void*, Garbage> Collection;
	OrE::ADT::HashMap Collection;

	// Nothing to do - standard constructor
	Collector() : Collection(50000, OrE::ADT::HM_PREFER_SIZE) {}

	// Log every non-freed memory
	~Collector()
	{
		if(!Collection.IsEmpty())
		{
			char acDesc[256];
			OrE::ADT::Iterator<OrE::ADT::Bucket> It( &Collection );
			while(++It)
			{
				sprintf(acDesc, "Memory leak detected. Allocation at ['%.23s': %d], %d bytes not freed.\n", ((Garbage*)It->pObject)->acFileName, ((Garbage*)It->pObject)->iLine, ((Garbage*)It->pObject)->iSize);

				#ifdef OR_GC_DEBUGOUT
				OutputDebugString(acDesc);
				#endif


				// If your program stops here you have a memory leak.
				// Use your debugger to see which memory is non-freed (content of iterator).
				// Your program also breaks because you enabled garbage collection by including
				// the OrDebug.h header. Remove the include if you don't wanna have garbage collection.
				// If you only wanna have the console or file output uncomment the OR_GC_BREAK in
				// the header file.
				#ifdef OR_GC_BREAK
				__debugbreak();
				#endif
			}
			// The leaks don't matter. We are in the program-dieing-phase were the OS will
			// remove all remaining.
			Collection.Clear();
		}
	}
};

static Collector Col;

// ******************************************************************************** //
// Operator overloading.
#undef new
int g_iInOperator = 0;
void* operator new(size_t sz, const char* pcFile, int iLine) {
	void* m = malloc(sz);
	//PGarbage G = PGarbage(m, Garbage(sz, pcFile, iLine));

	++g_iInOperator;
	Col.Collection.Insert(new Garbage(sz, pcFile, iLine), qword(m));
	--g_iInOperator;
	return m;
}

// This is the deletion kernel
inline void Delete(void* m)
{
	if(m)
	{
		if(!g_iInOperator)
		{
			++g_iInOperator;
			Col.Collection.Delete(qword(m));
			--g_iInOperator;
		}
		free(m);
	}
}

// This deletes are even used, if memory was not allocated with our
// new operator. The drawback is a little performance loss.
void operator delete(void* m)
{
	Delete(m);
}

void operator delete[](void* m)
{
	Delete(m);
}

void operator delete(void* m, const char* pcFile, int iLine)
{
	Delete(m);
}

void operator delete[](void* m, const char* pcFile, int iLine)
{
	Delete(m);
}

#endif	// _DEBUG
// *************************************EOF**************************************** //