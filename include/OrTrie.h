// ******************************************************************************** //
// OrTrie.h																			//
// ========																			//
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

#pragma once

namespace OrE {
namespace ADT {

// ******************************************************************************** //
struct TrieNode
{
private:
	TrieNode*				pNext;
	TrieNode*				pChild;
	char*					pSubString;		// Knotenbeschriftung
public:
	void*					pData;			// Beliebige Daten
	const char* GetSubString() const {return pSubString;}
	friend class Trie;
};
typedef TrieNode* TrieNodeP;

// ******************************************************************************** //
class Trie
{
private:
	// Oberster Knoten mit der leeren Zeichenkette
	TrieNodeP	m_pFirstNode;
	ADTDeleteObjectCallbackP m_pDeleteCallback;

	// Prevent copy constructor and operator = being generated.
	Trie(const Trie&);
	const Trie& operator = (const Trie&);
public:
	// Konstruktor
	Trie(): m_pDeleteCallback(0),m_pFirstNode(0)	{}


	// ******************************************************************************** //
	// DE																				//
	// Zweck: Fügt dem Trie neue Daten hinzu oder überschreibt alte.					//
	//	Es wird nur der übergebene Zeiger kopiert. Die Daten dürfen also				//
	//	nicht gelöscht werden.															//
	// Parameter:																		//
	//	[in] _pcName - String, der in den Trie eingegliedert wird und das				//
	//	Element identifiziert.															//
	//	[in] _pData - Gültiger Zeiger auf einen existierenden Datensatz.				//
	//	[in] _bOverrideData - Wenn gesetzt werden alle Daten auf dem Weg				//
	//		zum Blatt überschreiben. Andernfalls wir nur im letzten Knoten				//
	//		von _pcName die Daten gespeichert. Der Rückgabewert 1 wird					//
	//		nicht mehr ausgegeben, egal was passiert.									//
	// Rückgabe:																		//
	//	0 - alles OK																	//
	//	1 - Daten bereits vorhanden (wird überschrieben)								//
	//	2 - unbekannter Fehler															//
	//																					//
	// EN																				//
	// Object: Adds a new data to Trie or overwrites an older.							//
	//	The given pointer is copied. The original data may not be deleted.				//
	// Parameter:																		//
	//	[in] _pcName - String to identify the data in the Trie.							//
	//	[in] _pData - Pointer to the new data.											//
	//	[in] _bOverrideData - Override the data in all nodes on the way to				//
	//		the leaf. If set to false the data is just stored in the last				//
	//		node from the current name. That can also override the data in				//
	//		this last node (see return value).											//
	// Return:																			//
	//	0 - all correct																	//
	//	1 - there was an old dataset with the specified name, that was					//
	//	overwritten. If _bOverrideData==true this will not be reported (the				//
	//	return value is always 0 or 2).													//
	//	2 - unknown failure																//
	// ******************************************************************************** //
	int Add(const char* _pcName, void* _pData, bool _bOverrideData);



	// ******************************************************************************** //
	// DE																				//
	// Zweck: Finden die passenden Daten zu einem Namen.								//
	// Parameter:																		//
	//	[in/out] _ppcName - Name der Daten die gefunden werden soll. Nach				//	
	//	Ausführung Rest des Namens, wenn nur ein Teilstring								//
	//	gefunden werden konnte.															//
	//	Konnte gar kein match gefunden werden wird der vollständige Name				//
	//	erhalten.																		//
	// Rückgabe: Der gefundene Knoten (kann auch Prefix des								//
	//	Namen sein) oder 0, wenn gar kein Fund vorliegt.								//
	// Bemerkung: Groß-/Kleinschreibung wird beachtet!									//
	//																					//
	// EN																				//
	// Object: Find the matching data for the name.										//
	// Parameter:																		//
	//	[in/out] _ppcName - A Pointer to the name we are searching for.					//
	//	After return the pointer points to the remaining name, which doesn't			//
	//	match. Then there was only an existing string labeled with the					//
	//	prefix of the name.																//
	//	If there is absolutly no match, it would be the original pointer.				//
	// Return: The found node or 0 if there is no such function.						//
	// Note: Case sensitive!															//
	// ******************************************************************************** //
	TrieNodeP Match(const char** _ppcName);


	// ******************************************************************************** //
	// DE																				//
	// Zweck: Löscht alle Knoten, die nur durch einfügen dieses Knotens					//
	//	entstanden sind.																//
	// Parameter:																		//
	//	[in] _pcName - Name der gefunden werden soll.									//
	// Bemerkung: Groß-/Kleinschreibung wird beachtet!									//
	//																					//
	// EN																				//
	// Object: Delete all nodes which are only created throug add _pcName.				//
	// Parameter:																		//
	//	[in] _pcName - A Pointer to the name we are searching for.						//
	// Note: Case sensitive!															//
	// ******************************************************************************** //
	void Remove(const char* _pcName, void* _pData);


	// ******************************************************************************** //
	// DE																				//
	// Zweck: Wird beim Löschen eines Datansatzes angewendet.							//
	// Parameter:																		//
	//	[in] _pDeleteCallback - Pointer auf eine Funktion, die einen					//
	//	Datansatz löschen kann.															//
	//																					//
	// EN																				//
	// Object: Is used during release of a dataset.										//
	// Parameter:																		//
	//	[in] _pDeleteCallback - Pointer to a function, which releases a					//
	//	dataset.																		//
	// ******************************************************************************** //
	void SetDeleteCallback(ADTDeleteObjectCallbackP _pDeleteCallback)
	{
		m_pDeleteCallback = _pDeleteCallback;
	}

	// ******************************************************************************** //
	// DE																				//
	// Zweck: Gibt allen vom Trie verwendeten Speicher frei. Der Endzustand				//
	//	entspricht dem Startzustand (leerer Trie).										//
	//	Die Funktion löscht rekursiv.													//
	// Parameter:																		//
	//	[in] _pNode - Der zu löschende Knoten. Wird durch rekursion belegt				//
	//	und darf nicht angegeben werden! Korrekter aufruf: ReleaseTrie();				//
	//																					//
	// EN																				//
	// Object: Release all memory. The result is an empty Trie like to					//
	//	begin.																			//
	//	It is a recursiv process.														//
	// Parameter:																		//
	//	[in] _pNode - The current node to delete. Is set by recursion.					//
	//	Don't declare this parameter. Correct use: ReleaseTrie();						//
	// ******************************************************************************** //
	void ReleaseTrie(TrieNodeP _pNode = 0);

	~Trie()	{ReleaseTrie();}
};


	
	
// ******************************************************************************** //
// Binary data string trie.

// String class to reprsent binary strings containing 0 and other chars.
/*class TrieString
{
private:
	bool m_bDel;

	TrieString(dword _dwLen, char *_pcString):m_pcString(_pcString),m_dwLen(_dwLen),m_bDel(true)	{}
public:
	dword m_dwLen;
	const char* m_pcString;

	TrieString(const char *_pcString, dword _dwLen):m_pcString(_pcString),m_dwLen(_dwLen),m_bDel(false)	{}
	virtual ~TrieString();

	// ******************************************************************************** //
	// Auxiliary function to create a copy of a subsrting.
	// Parameter:
	//	_dwFrom - 0-indexed imdex of first char to copy (inclusive)
	//	_dwTo - 0-indexed imdex of last char to copy (inclusive)
	//			or 0xffffffff to copy the whole postfix begining in _dwFrom
	TrieString* substr(const dword _dwFrom, dword _dwTo) const;

	// ******************************************************************************** //
	// Auxiliary function to estimate length of the prefix match
	dword match(const TrieString* _pStr) const;
};

// ******************************************************************************** //
struct TrieNode
{
	TrieNode*				pNext;
	TrieNode*				pChild;
	TrieString*				pSubString;		// Knotenbeschriftung

	void*					pData;			// Beliebige Daten
};
typedef TrieNode* TrieNodeP;

// ******************************************************************************** //
class Trie
{
private:
	// Oberster Knoten mit der leeren Zeichenkette
	TrieNodeP	m_pFirstNode;
	ADTDeleteObjectCallbackP m_pDeleteCallback;
public:
	// Konstruktor
	Trie(): m_pDeleteCallback(0),m_pFirstNode(0)	{}


	// ******************************************************************************** //
	// DE																				//
	// Zweck: Fügt dem Trie neue Daten hinzu oder überschreibt alte.					//
	//	Es wird nur der übergebene Zeiger kopiert. Die Daten dürfen also				//
	//	nicht gelöscht werden.															//
	// Parameter:																		//
	//	[in] _pcName - String, der in den Trie eingegliedert wird und das				//
	//	Element identifiziert.															//
	//	[in] _pData - Gültiger Zeiger auf einen existierenden Datensatz.				//
	//	[in] _bOverrideData - Wenn gesetzt werden alle Daten auf dem Weg				//
	//		zum Blatt überschreiben. Andernfalls wir nur im letzten Knoten				//
	//		von _pcName die Daten gespeichert. Der Rückgabewert 1 wird					//
	//		nicht mehr ausgegeben, egal was passiert.									//
	// Rückgabe:																		//
	//	0 - alles OK																	//
	//	1 - Daten bereits vorhanden (wird überschrieben)								//
	//	2 - unbekannter Fehler															//
	//																					//
	// EN																				//
	// Object: Adds a new data to Trie or overwrites an older.							//
	//	The given pointer is copied. The original data may not be deleted.				//
	// Parameter:																		//
	//	[in] _pcName - String to identify the data in the Trie.							//
	//	[in] _pData - Pointer to the new data.											//
	//	[in] _bOverrideData - Override the data in all nodes on the way to				//
	//		the leaf. If set to false the data is just stored in the last				//
	//		node from the current name. That can also override the data in				//
	//		this last node (see return value).											//
	// Return:																			//
	//	0 - all correct																	//
	//	1 - there was an old dataset with the specified name, that was					//
	//	overwritten. If _bOverrideData==true this will not be reported (the				//
	//	return value is always 0 or 2).													//
	//	2 - unknown failure																//
	// ******************************************************************************** //
	int Add(TrieString _Name, void* _pData, bool _bOverrideData);



	// ******************************************************************************** //
	// DE																				//
	// Zweck: Finden die passenden Daten zu einem Namen.								//
	// Parameter:																		//
	//	[in/out] _ppcName - Name der Funktion die gefunden werden soll. Nach			//	
	//	Ausführung Rest des Namens, wenn nur ein Teilstring								//
	//	gefunden werden konnte.															//
	//	Konnte gar kein match gefunden werden wird der vollständige Name				//
	//	erhalten.																		//
	// Rückgabe: Der gefundene Knoten (kann auch Teilfunktion Prefix des				//
	//	Namen sein) oder 0, wenn gar kein Fund vorliegt.								//
	// Bemerkung: Groß-/Kleinschreibung wird beachtet!									//
	//																					//
	// EN																				//
	// Object: Find the matching data for the name.										//
	// Parameter:																		//
	//	[in/out] _ppcName - A Pointer to the name we are searching for.					//
	//	After return the pointer points to the remaining name, which doesn't			//
	//	match. Then there was only an existing string labeled with the					//
	//	prefix of the name.																//
	//	If there is absolutly no match, it would be the original pointer.				//
	// Return: The found node or 0 if there is no such function.						//
	// Note: Case sensitive!															//
	// ******************************************************************************** //
	TrieNodeP Match(TrieString& _Name);


	// ******************************************************************************** //
	// DE																				//
	// Zweck: Löscht alle Knoten, die nur durch einfügen dieses Knotens					//
	//	entstanden sind.																//
	// Parameter:																		//
	//	[in] _pcName - Name der gefunden werden soll.									//
	// Bemerkung: Groß-/Kleinschreibung wird beachtet!									//
	//																					//
	// EN																				//
	// Object: Delete all nodes which are only created throug add _pcName.				//
	// Parameter:																		//
	//	[in] _pcName - A Pointer to the name we are searching for.						//
	// Note: Case sensitive!															//
	// ******************************************************************************** //
	void Remove(TrieString& _Name, void* _pData);


	// ******************************************************************************** //
	// DE																				//
	// Zweck: Wird beim Löschen eines Datansatzes angewendet.							//
	// Parameter:																		//
	//	[in] _pDeleteCallback - Pointer auf eine Funktion, die einen					//
	//	Datansatz löschen kann.															//
	//																					//
	// EN																				//
	// Object: Is used during release of a dataset.										//
	// Parameter:																		//
	//	[in] _pDeleteCallback - Pointer to a function, which releases a					//
	//	dataset.																		//
	// ******************************************************************************** //
	void SetDeleteCallback(ADTDeleteObjectCallbackP _pDeleteCallback)
	{
		m_pDeleteCallback = _pDeleteCallback;
	}

	// ******************************************************************************** //
	// DE																				//
	// Zweck: Gibt allen vom Trie verwendeten Speicher frei. Der Endzustand				//
	//	entspricht dem Startzustand (leerer Trie).										//
	//	Die Funktion löscht rekursiv.													//
	// Parameter:																		//
	//	[in] _pNode - Der zu löschende Knoten. Wird durch rekursion belegt				//
	//	und darf nicht angegeben werden! Korrekter aufruf: ReleaseTrie();				//
	//																					//
	// EN																				//
	// Object: Release all memory. The result is an empty Trie like to					//
	//	begin.																			//
	//	It is a recursiv process.														//
	// Parameter:																		//
	//	[in] _pNode - The current node to delete. Is set by recursion.					//
	//	Don't declare this parameter. Correct use: ReleaseTrie();						//
	// ******************************************************************************** //
	void ReleaseTrie(TrieNodeP _pNode = 0);

	~Trie()	{ReleaseTrie();}
};*/


}; // namespace ADT
}; // namespace OrE
// *************************************EOF**************************************** //