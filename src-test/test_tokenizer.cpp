// ******************************************************************************** //
// test_tokenizer.cpp																//
// ==================																//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: Testing of the class OrE::String::Tokenizer.							//
//																					//
// ******************************************************************************** //

#include "../include/OrBaseLib.h"
#include "../include/OrDebug.h"
#include <iostream>

typedef OrE::String::Tokenizer<wchar_t> UTF16Tokenizer;

const wchar_t* TOKENIZER_TEST_STRING_1 = L"Text zum Testen des Tokenizers(wirklich!),\nDer WAHNSINN...";

void test_tokenizer()
{
	UTF16Tokenizer Tok;
	Tok.Add( UTF16Tokenizer::Delimiter( L'.' ) );
	Tok.Add( UTF16Tokenizer::StringDelimiters::WHITE_SPACES, UTF16Tokenizer::StringDelimiters::NUM_WHITE_SPACES );
	Tok.Add( UTF16Tokenizer::Delimiter( L',' ) );
	Tok.Add( UTF16Tokenizer::Delimiter( L',' ) );
	Assert( Tok.GetNumDelimiters()==6 );

	UTF16Tokenizer::TokenArray T = Tok.Tokenize( TOKENIZER_TEST_STRING_1, wcslen(TOKENIZER_TEST_STRING_1) );
	for( auto t : T )
		std::cout << char(TOKENIZER_TEST_STRING_1[t.iStart]) << "\t\t" << t.iLength << '\n';

	std::cout << '\n';
}