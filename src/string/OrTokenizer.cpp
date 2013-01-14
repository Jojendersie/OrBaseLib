// ******************************************************************************** //
// OrTokenizer.cpp																	//
// ===============																	//
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

#include "../../include/OrTypeDef.h"
#include "../../include/string/OrTokenizer.h"

// All white spaces: \r, \n, ' ', \t
const OrE::String::Tokenizer<char>::Delimiter OrE::String::Tokenizer<char>::StringDelimiters::WHITE_SPACES[] =
	{ OrE::String::Tokenizer<char>::Delimiter(' ', true),
	  OrE::String::Tokenizer<char>::Delimiter('\t', true),
	  OrE::String::Tokenizer<char>::Delimiter('\n'),
	  OrE::String::Tokenizer<char>::Delimiter('\r')
	};
const OrE::String::Tokenizer<wchar_t>::Delimiter OrE::String::Tokenizer<wchar_t>::StringDelimiters::WHITE_SPACES[] =
	{ OrE::String::Tokenizer<wchar_t>::Delimiter(L' ', true),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'\t', true),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'\n'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'\r')
	};


// All brackets: (, ), [, ], {, }
const OrE::String::Tokenizer<char>::Delimiter OrE::String::Tokenizer<char>::StringDelimiters::BRACKETS[] =
	{ OrE::String::Tokenizer<char>::Delimiter('('),
	  OrE::String::Tokenizer<char>::Delimiter(')'),
	  OrE::String::Tokenizer<char>::Delimiter('['),
	  OrE::String::Tokenizer<char>::Delimiter(']'),
	  OrE::String::Tokenizer<char>::Delimiter('{'),
	  OrE::String::Tokenizer<char>::Delimiter('}')
	};
const OrE::String::Tokenizer<wchar_t>::Delimiter OrE::String::Tokenizer<wchar_t>::StringDelimiters::BRACKETS[] =
	{ OrE::String::Tokenizer<wchar_t>::Delimiter(L'('),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L')'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'['),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L']'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'{'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'}')
	};


// Operators of C-like languages: ^, ", %, &, /, |, <, >, =, ?, :, ',', '.', +, *, ~, -, !, ;, #
const OrE::String::Tokenizer<char>::Delimiter OrE::String::Tokenizer<char>::StringDelimiters::OPERATORS[] =
	{ OrE::String::Tokenizer<char>::Delimiter('^'),
	  OrE::String::Tokenizer<char>::Delimiter('"'),
	  OrE::String::Tokenizer<char>::Delimiter('%'),
	  OrE::String::Tokenizer<char>::Delimiter('&'),
	  OrE::String::Tokenizer<char>::Delimiter('/'),
	  OrE::String::Tokenizer<char>::Delimiter('|'),
	  OrE::String::Tokenizer<char>::Delimiter('<'),
	  OrE::String::Tokenizer<char>::Delimiter('>'),
	  OrE::String::Tokenizer<char>::Delimiter('='),
	  OrE::String::Tokenizer<char>::Delimiter('?'),
	  OrE::String::Tokenizer<char>::Delimiter(':'),
	  OrE::String::Tokenizer<char>::Delimiter('.'),
	  OrE::String::Tokenizer<char>::Delimiter(','),
	  OrE::String::Tokenizer<char>::Delimiter(';'),
	  OrE::String::Tokenizer<char>::Delimiter('+'),
	  OrE::String::Tokenizer<char>::Delimiter('*'),
	  OrE::String::Tokenizer<char>::Delimiter('~'),
	  OrE::String::Tokenizer<char>::Delimiter('-'),
	  OrE::String::Tokenizer<char>::Delimiter('!'),
	  OrE::String::Tokenizer<char>::Delimiter('#')
	};
const OrE::String::Tokenizer<wchar_t>::Delimiter OrE::String::Tokenizer<wchar_t>::StringDelimiters::OPERATORS[] =
	{ OrE::String::Tokenizer<wchar_t>::Delimiter(L'^'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'"'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'%'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'&'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'/'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'|'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'<'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'>'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'='),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'?'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L':'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'.'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L','),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L';'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'+'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'*'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'~'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'-'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'!'),
	  OrE::String::Tokenizer<wchar_t>::Delimiter(L'#')
	};


// *************************************EOF**************************************** //