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
#include <wctype.h>

typedef OrE::String::Tokenizer<wchar_t> UTF16Tokenizer;

const wchar_t* TOKENIZER_TEST_STRING_1 = L"Text zum Testen des Tokenizers(\"wirklich\"!),\nDer WAHNSINN... 1e3f, 103.13, .1f, 0.f";


enum struct C_TOKEN_TYPES
{
	UNKNOWN = 0,
	ERROR_TYPE,
	INTEGER,
	FLOAT,
	DOUBLE,
	FLOAT_END,			// #e#f | #f
	DOUBLE_END,			// #e#
	CLASS,
	STRUCT,
	ENUM,
	UNION,
	IDENTIFIER,
	CONST,
	STATIC,
	TYPEDEF,
	FUNCTION,
	OPEN_SCOPE,			// {
	CLOSE_SCOPE,		// }
	COMMENT_LINE_START,	// //
	COMMENT_START,		// /*
	COMMENT_END,		// */
	OPEN_BRAKET,		// ( | [
	CLOSE_BRAKET		// ) | ]
};

// If a token starts with a digit test the rest:
// - if all other are digits -> INTEGER
// - all numbers or maximal one e or E:
//	- if the last is an f -> FLOAT_END
//	- if the last is an d -> DOUBLE_END
int32 NumberType( int s, int l, const wchar_t* _pArray )
{
	if( l==0 ) return int32(C_TOKEN_TYPES::INTEGER);
	bool bHasExponent = false;
	for( int i=s; i<l-1; ++i )
		if( !iswdigit(_pArray[i]) )
			if( !bHasExponent && (_pArray[i] == L'e' || _pArray[i] == L'e') )
				bHasExponent = true;
			else return int32(C_TOKEN_TYPES::ERROR_TYPE);
	wchar_t cLast = _pArray[s+l-1];
	if( iswdigit(cLast) ) return bHasExponent ? int32(C_TOKEN_TYPES::DOUBLE_END) : int32(C_TOKEN_TYPES::INTEGER);
	if( cLast == 'f' && iswdigit(_pArray[s+l-2]) ) return int32(C_TOKEN_TYPES::FLOAT_END);
}


int32 IdentifyCToken( const OrE::String::Tokenizer<wchar_t>::Token& _Token, const wchar_t* _pArray )
{
	if( iswdigit( _pArray[_Token.iStart] ) ) return NumberType( _Token.iStart+1, _Token.iLength-1, _pArray );
}


// Look ahead tokenizer
//int32 t()
//{
	//C_TOKEN_TYPES Type = IdentifyCToken();
	//switch( Type )
//	{
//	case C_TOKEN_TYPES::INTEGER:	// Look ahead for a point and the floating point ending
//	case C_TOKEN_TYPES::DIV:		// Look ahead for a /* or // or /=
//	case C_TOKEN_TYPES::MUL:		// Look ahead for a */ or *=
//	case C_TOKEN_TYPES::ADD:		// +=
//	case C_TOKEN_TYPES::SUB:		// -= or ->
//	case C_TOKEN_TYPES::DQUOTE:		// Search up to the next "
//	case C_TOKEN_TYPES::QUOTE:		// Search up to the next '
//	case C_TOKEN_TYPES::OR:			// logical or || or |=
//	case C_TOKEN_TYPES::AND:		// logical and && or &=
//	case C_TOKEN_TYPES::XOR:		// ^=
//	case C_TOKEN_TYPES::NOT:		// !=
//	case C_TOKEN_TYPES::LESS:		// left shift <<, less equal <=, <<=
//	case C_TOKEN_TYPES::GREATER:	// right shift >>, greater equal >=, >>=
//	case C_TOKEN_TYPES::ASSIGN:		// equal ==
//	case C_TOKEN_TYPES::DCOLON:		// Scope access ::
//	case C_TOKEN_TYPES::POINT:		// .0f, .0235e4
//		C_TOKEN_TYPES t = IdentifyCToken(nextone);
//		if( t==C_TOKEN_TYPES::FLOAT_END ) Type = C_TOKEN_TYPES::FLOAT;	// TODO: del next token? or return correct ones (typised tokens)
//		if( t==C_TOKEN_TYPES::DOUBLE_END || t==C_TOKEN_TYPES::INTEGER ) Type = C_TOKEN_TYPES::DOUBLE;
//	}

//	return int32(Type);
//}


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