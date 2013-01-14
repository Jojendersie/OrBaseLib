// ******************************************************************************** //
// OrTokenizer.h																	//
// =============																	//
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
// This unit provides a simple tokenizer which splits arbitrary arrays into tokens.	//
// There are two types of delimiters. Hidden ones do no not create tokens but split //
// and visible ones get there own tokens.											//
// ******************************************************************************** //

#pragma once

#include <vector>

namespace OrE {
namespace String {

	template <typename T> class Tokenizer
	{
	public:
		// ******************************************************************************** //
		// A simple token. This saves the start index of the first character and
		// the length of the token. The length is at least one.
		struct Token
		{
			int iStart;
			int iLength;

			Token( int s, int l ) : iStart(s), iLength(l)	{}
		};

		typedef std::vector<Token> TokenArray;

		// ******************************************************************************** //
		// This token has a type. The start and the length are the same as in the previous
		// token.
		struct TypecastedToken
		{
			uint32 uiStart;
			uint32 uiLength;
			int32 iType;

			TypecastedToken( uint32 s, uint32 l, int32 t ) : uiStart(s), uiLength(l), iType(t)	{}
		};

		typedef std::vector<TypecastedToken> TypecastedTokenArray;

		// ******************************************************************************** //
		// One single delimiter. The tokenizer uses at least one delimiter to split the array.
		class Delimiter
		{
		private:
			T m_Value;
			bool m_bHidden;
		public:
			Delimiter( const T& _Value, bool _bHidden = false ) : m_Value( _Value ), m_bHidden( _bHidden )	{}

			bool IsHidden() const						{return m_bHidden;}

			operator T& ()								{return m_Value;}
			operator const T& () const					{return m_Value;}

			// ******************************************************************************** //
			// Comparison operators - very straight forward, just shortens the access to m_Value.
			inline bool operator == (const T& b) const	{return m_Value == b;}
			inline bool operator != (const T& b) const	{return m_Value != b;}
			inline bool operator < (const T& b) const	{return m_Value < b;}
			inline bool operator <= (const T& b) const	{return m_Value <= b;}
			inline bool operator > (const T& b) const	{return m_Value > b;}
			inline bool operator >= (const T& b) const	{return m_Value >= b;}
			// Only one for Delimiter <-> Delimiter used to avoid double insertion
			//inline bool operator != (const Delimiter& b) const	{return m_Value != b.m_Value;}
		};

	private:
		// Array of sorted delimiters. The sorting improves performance through a linear search
		// during character identification.
		std::vector<Delimiter> m_aDelimiters;

		// Internal variables for iterative tokenization
		const T* m_pArray;
		int m_iArraySize;
		int m_iCursor;

		// Method to check if the current character is a delimiter or not. Its visibility is returned
		// in the _bVis parameter. _bVis is undefined/not changed if the character is no delimiter.
		bool IsDelimiter( const T& _Char, bool& _bVis )
		{
			// Binary search in the sorted delimiter array.
			int iFirst = 0;
			int iLast = m_aDelimiters.size()-1;
			while( iLast >= iFirst )
			{
				int iM = (iFirst + iLast) / 2;
				if( m_aDelimiters[iM] < _Char ) iFirst = iM+1;
				else if( m_aDelimiters[iM] > _Char ) iLast = iM-1;
				else {_bVis = !m_aDelimiters[iM].IsHidden(); return true;}
			}
			return false;
		}

	public:
		Tokenizer() : m_pArray(nullptr), m_iArraySize(0), m_iCursor(0)
		{
		}

		// Add one delimiter to the tokenizer. The function takes one
		// delimiter and adds it O(log n).
		void Add( const Delimiter& _D )
		{
			// Search insertion point.
			auto It = m_aDelimiters.begin();
			while( (It != m_aDelimiters.end()) && (*It < _D) ) ++It;
			// Insert
			//if( _D != (const Delimiter&)It )
			if( It==m_aDelimiters.end() || *It != _D )
				m_aDelimiters.insert( It, _D );
		}

		// Add multiple delimiters to the tokenizer. This function calls the other Add for
		// each element. It is just to shorten initialization.
		void Add( const Delimiter* _aD, int _iNum )
		{
			while( _iNum-- )
			{
				Add(*_aD);
				++_aD;
			}
		}

		int GetNumDelimiters()	{return m_aDelimiters.size();}

		// ******************************************************************************** //
		// The method to split an array. The tokenizer must be initialized. The returned token array
		// has to be deleted by the user.
		TokenArray Tokenize( const T* _pArray, int _iSize )
		{
			TokenArray Tokens;
			// The current token growth during iteration. At start it has no size yet.
			int iStart = 0;
			int iSize = 0;
			for( int i=0; i<_iSize; ++i )
			{
				bool bVis = false;
				if( IsDelimiter( *_pArray, bVis ) )
				{
					// Close up the last token
					if( iSize>0 ) Tokens.push_back( Token(iStart,iSize) );

					// Add a token for the delimiter
					if( bVis ) Tokens.push_back( Token(i,1) );

					// Initialize for the next token
					iSize = 0;
					iStart = i+1;
				} else
					++iSize;
				++_pArray;
			}
			return Tokens;
		}


		// ******************************************************************************** //
		// This methods gives the ability to iterate over the tokens. Call TokenizeIterative to
		// initialize the tokenizer. Make sure you are the only thread using this Tokenizer
		// object. Afterwards you can call GetNextToken() successive. If the array ends the
		// returned token has the length 0. Invisible delimters are skipped.
		void TokenizeIterative( const T* _pArray, int _iSize )
		{
			m_iArraySize = _iSize;
			m_iCursor = 0;
			m_pArray = _pArray;
		}

		Token GetNextToken()
		{
			// Stop at end of array, or if not initialized
			if( m_iArraySize <= m_iCursor || !m_pArray ) return Token( 0, 0 );
			TODO: return next token
		}


		// ******************************************************************************** //
		// Delimiters of type char and wchar_t
		class StringDelimiters { public:
			// All white spaces
			// Visible: \r, \n
			// Invisible: ' ', \t
			static const Delimiter WHITE_SPACES[];
			static const int NUM_WHITE_SPACES = 4;

			// All brackets
			// Visible: (, ), [, ], {, }
			// Invisible:
			static const Delimiter BRACKETS[];
			static const int NUM_BRACKETS = 6;

			// Operators of C-like languages
			// Visible: ^, ", %, &, /, |, <, >, =, ?, :, ',', '.', +, *, ~, -, !, ;, #
			// Invisible:
			static const Delimiter OPERATORS[];
			static const int NUM_OPERATORS = 20;
		};
	};


}; // namespace String
}; // namespace OrE

// *************************************EOF**************************************** //