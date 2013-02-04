/*
Copyright (c) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "../include/OrTypeDef.h"
#include "../include/OrFastMath.h"
#include "../include/OrComplexNumber.h"
#include "../include/OrFFT.h"
#include <cstdlib>

// ******************************************************************************** //
// Info structure to combine non changing attributes and lead them through the recursion
struct StaticState
{
	bool bInverse;
	int iNum;
	int iInitialStride;
	int aiFactors[64];
	OrE::Math::ComplexNumberP pFTFactors;
};

// ******************************************************************************** //
// Calculation of the Fourier transformation factors
void ComputeFTFactors(OrE::Math::ComplexNumberP* _pFTFactors, int _iNum, bool _bInverse) {
	*_pFTFactors = (OrE::Math::ComplexNumberP)malloc(_iNum * sizeof(OrE::Math::ComplexNumber));
	if( !_pFTFactors ) return;	// TODO: Fehlermeldung
	// Calculate 2*pi*i/n factors
	const float fFac = OrE::Math::f2Pi / _iNum;
    for(int i=0; i<_iNum; ++i) {
        float fPhase = fFac * i;
		if(_bInverse)
			fPhase = -fPhase;
		(*_pFTFactors)[i].r = cos(fPhase);
		(*_pFTFactors)[i].i = sin(fPhase);
    }
}

// ******************************************************************************** //
// Fast versions for some prime factors of the size
void kf_bfly2(
	OrE::Math::ComplexNumberP _pOut,
	const int _iStride,
	int m,
	const StaticState& _State)
{
    OrE::Math::ComplexNumberP pOut2 = _pOut + m;
	OrE::Math::ComplexNumberP pFTF = _State.pFTFactors;
    OrE::Math::ComplexNumber t;
    do {
		t = *pOut2 * *pFTF;
        pFTF += _iStride;
		*pOut2 = *_pOut - t;
		*_pOut += t;
        ++pOut2;
        ++_pOut;
    } while(--m);
}

// ******************************************************************************** //
void kf_bfly4(
	OrE::Math::ComplexNumberP _pOut,
	const int _iStride,
	int m,
	const StaticState& _State)
{
    OrE::Math::ComplexNumberP tw1,tw2,tw3;
    OrE::Math::ComplexNumber scratch[6];
    size_t k=m;
    const size_t m2=2*m;
    const size_t m3=3*m;


	tw3 = tw2 = tw1 = _State.pFTFactors;

    do {
        scratch[0] = _pOut[m] * *tw1;
        scratch[1] = _pOut[m2] * *tw2;
        scratch[2] = _pOut[m3] * *tw3;

        scratch[5] = *_pOut - scratch[1];
		*_pOut += scratch[1];
        scratch[3] = scratch[0] + scratch[2];
        scratch[4] = scratch[0] - scratch[2];
        _pOut[m2] = *_pOut - scratch[3];
        tw1 += _iStride;
        tw2 += _iStride*2;
        tw3 += _iStride*3;
        *_pOut += scratch[3];

        if(_State.bInverse) {
            _pOut[m].r = scratch[5].r + scratch[4].i;
            _pOut[m].i = scratch[5].i - scratch[4].r;
            _pOut[m3].r = scratch[5].r - scratch[4].i;
            _pOut[m3].i = scratch[5].i + scratch[4].r;
        }else{
            _pOut[m].r = scratch[5].r - scratch[4].i;
            _pOut[m].i = scratch[5].i + scratch[4].r;
            _pOut[m3].r = scratch[5].r + scratch[4].i;
            _pOut[m3].i = scratch[5].i - scratch[4].r;
        }
        ++_pOut;
    } while(--k);
}

// ******************************************************************************** //
void kf_bfly3(
	OrE::Math::ComplexNumberP _pOut,
	const int _iStride,
	int m,
	const StaticState& _State)
{
     size_t k=m;
     const size_t m2 = 2*m;
     OrE::Math::ComplexNumberP tw1,tw2;
     OrE::Math::ComplexNumber scratch[5];
     OrE::Math::ComplexNumber epi3;
	 epi3 = _State.pFTFactors[_iStride*m];

	 tw1=tw2=_State.pFTFactors;

     do{
         scratch[1] = _pOut[m] * *tw1;
         scratch[2] = _pOut[m2] * *tw2;

         scratch[3] = scratch[1] + scratch[2];
         scratch[0] = scratch[1] - scratch[2];
         tw1 += _iStride;
         tw2 += _iStride*2;

         _pOut[m].r = _pOut->r - scratch[3].r * 0.5f;
         _pOut[m].i = _pOut->i - scratch[3].i * 0.5f;

         scratch[0] *= epi3.i;

         *_pOut += scratch[3];

         _pOut[m2].r = _pOut[m].r + scratch[0].i;
         _pOut[m2].i = _pOut[m].i - scratch[0].r;

         _pOut[m].r -= scratch[0].i;
         _pOut[m].i += scratch[0].r;

         ++_pOut;
     } while(--k);
}

// ******************************************************************************** //
void kf_bfly5(
	OrE::Math::ComplexNumberP _pOut,
	const int _iStride,
	int m,
	const StaticState& _State)
{
    OrE::Math::ComplexNumberP pOut0,pOut1,pOut2,pOut3,pOut4;
    int u;
    OrE::Math::ComplexNumber scratch[13];
    OrE::Math::ComplexNumber ya,yb;
	ya = _State.pFTFactors[_iStride*m];
    yb = _State.pFTFactors[_iStride*2*m];

    pOut0=_pOut;
    pOut1=pOut0+m;
    pOut2=pOut0+2*m;
    pOut3=pOut0+3*m;
    pOut4=pOut0+4*m;

    for ( u=0; u<m; ++u ) {
        scratch[0] = *pOut0;

        scratch[1] = *pOut1 * _State.pFTFactors[u*_iStride];
        scratch[2] = *pOut2 * _State.pFTFactors[2*u*_iStride];
        scratch[3] = *pOut3 * _State.pFTFactors[3*u*_iStride];
        scratch[4] = *pOut4 * _State.pFTFactors[4*u*_iStride];

        scratch[7] = scratch[1] + scratch[4];
        scratch[10] = scratch[1] - scratch[4];
        scratch[8] = scratch[2] + scratch[3];
        scratch[9] = scratch[2] - scratch[3];

        pOut0->r += scratch[7].r + scratch[8].r;
        pOut0->i += scratch[7].i + scratch[8].i;

        scratch[5].r = scratch[0].r + scratch[7].r*ya.r + scratch[8].r*yb.r;
        scratch[5].i = scratch[0].i + scratch[7].i*ya.r + scratch[8].i*yb.r;

        scratch[6].r =   scratch[10].i*ya.i + scratch[9].i*yb.i;
        scratch[6].i = - scratch[10].r*ya.i - scratch[9].r*yb.i;

        *pOut1 = scratch[5] - scratch[6];
        *pOut4 = scratch[5] + scratch[6];

        scratch[11].r = scratch[0].r + scratch[7].r*yb.r + scratch[8].r+ya.r;
        scratch[11].i = scratch[0].i + scratch[7].i*yb.r + scratch[8].i+ya.r;
        scratch[12].r = - scratch[10].i*yb.i + scratch[9].i*ya.i;
        scratch[12].i = scratch[10].r*yb.i - scratch[9].r*ya.i;

        *pOut2 = scratch[11] + scratch[12];
        *pOut3 = scratch[11] - scratch[12];

        ++pOut0;++pOut1;++pOut2;++pOut3;++pOut4;
    }
}

// ******************************************************************************** //
/* perform the butterfly for one stage of a mixed radix FFT */
void kf_bfly_generic(
	OrE::Math::ComplexNumberP _pOut,
	const int _iStride,
	int m,
	int p,
	const StaticState& _State)
{
	OrE::Math::ComplexNumberP pScratch = (OrE::Math::ComplexNumberP)malloc(sizeof(OrE::Math::ComplexNumber)*p);

    for (int u=0; u<m; ++u ) {
        int k=u;
        for (int q=0 ; q<p ; ++q ) {
            pScratch[q] = _pOut[k];
            k += m;
        }

        k=u;
        for (int q1=0 ; q1<p ; ++q1 ) {
            int iFactorIdx = 0;
            _pOut[k] = pScratch[0];
            for(int q=1; q<p; ++q ) {
                iFactorIdx += _iStride * k;
				if (iFactorIdx>=_State.iNum) iFactorIdx -= _State.iNum;
				_pOut[k] += pScratch[q] * _State.pFTFactors[iFactorIdx];
            }
            k += m;
        }
    }
    free(pScratch);
}

// ******************************************************************************** //
void FFTKernel(
	OrE::Math::ComplexNumberP _pOut,
	const OrE::Math::ComplexNumber* _pIn,
	const int _iStride,
	int * _piFactors,
	const StaticState& _State)
{
	OrE::Math::ComplexNumberP pOut = _pOut;
    const int p=*_piFactors++; // the radix 
    const int m=*_piFactors++; // stage's fft length/p 
    const OrE::Math::ComplexNumberP pOutEnd = _pOut + p*m;

    if (m==1) {
        do{
            *pOut = *_pIn;
            _pIn += _iStride;
        } while(++pOut != pOutEnd);
    }else{
        do{
            // recursive call:
            // DFT of size m*p performed by doing
            // p instances of smaller DFTs of size m, 
            // each one takes a decimated version of the input
            FFTKernel( pOut , _pIn, _iStride*p, _piFactors, _State);
            _pIn += _iStride;
        } while( (pOut += m) != pOutEnd );
    }

    // recombine the p smaller DFTs 
//	kf_bfly_generic(_pOut, _iStride/_State.iInitialStride, m, p, _State);
    switch (p) {
		case 2: kf_bfly2(_pOut, _iStride/_State.iInitialStride, m, _State); break;
		case 3: kf_bfly3(_pOut, _iStride/_State.iInitialStride, m, _State); break; 
		case 4: kf_bfly4(_pOut, _iStride/_State.iInitialStride, m, _State); break;
		case 5: kf_bfly5(_pOut, _iStride/_State.iInitialStride, m, _State); break; 
        default: kf_bfly_generic(_pOut, _iStride/_State.iInitialStride, m, p, _State); break;
    }//*/
}

// ******************************************************************************** //
// Factorize a integer number
void Factorize(int _iNum, int* _aiFactors)
{
    int p=4;
	int iFloorSqrt = OrE::Math::Floor( OrE::Math::Sqrt((float)_iNum) );

    /*factor out powers of 4, powers of 2, then any remaining primes */
    do {
        while (_iNum % p) {
            switch (p) {
                case 4: p = 2; break;
                case 2: p = 3; break;
                default: p += 2; break;
            }
            if (p > iFloorSqrt)
                p = _iNum;          /* no more factors, skip to end */
        }
        _iNum /= p;
        *_aiFactors++ = p;
        *_aiFactors++ = _iNum;
    } while (_iNum > 1);
}

// ******************************************************************************** //
// The visible FFT functions
// ******************************************************************************** //
OrE::Math::ComplexNumberP OrE::Math::fft(const float* _pfInput, int _iNum)
{
	// Alloc return memory
	OrE::Math::ComplexNumberP pResult = (OrE::Math::ComplexNumberP)malloc(_iNum * sizeof(OrE::Math::ComplexNumber));
	OrE::Math::ComplexNumberP pIn = (OrE::Math::ComplexNumberP)malloc(_iNum * sizeof(OrE::Math::ComplexNumber));
	// Precompute all factors and set static informations
	StaticState State;
	State.iInitialStride = 1; State.iNum = _iNum; State.bInverse = false;
    Factorize(_iNum, State.aiFactors);
	ComputeFTFactors(&State.pFTFactors, _iNum, false);

	for(int i=0; i<_iNum; ++i) {
		pIn[i].r = _pfInput[i];
		pIn[i].i = 0.0f;
    }

	FFTKernel(pResult, pIn, 1, State.aiFactors, State);

	free(State.pFTFactors);
	free(pIn);

	return pResult;
}

// ******************************************************************************** //
OrE::Math::ComplexNumberP OrE::Math::fft(const OrE::Math::ComplexNumber* _pInput, int _iNum)
{
	// Alloc return memory
	OrE::Math::ComplexNumberP pResult = (OrE::Math::ComplexNumberP)malloc(_iNum * sizeof(OrE::Math::ComplexNumber));
	// Precompute all factors and set static informations
	StaticState State;
	State.iInitialStride = 1; State.iNum = _iNum; State.bInverse = false;
    Factorize(_iNum, State.aiFactors);
	ComputeFTFactors(&State.pFTFactors, _iNum, false);

	FFTKernel(pResult, _pInput, 1, State.aiFactors, State);

	free(State.pFTFactors);

	return pResult;
}

// ******************************************************************************** //
OrE::Math::ComplexNumberP OrE::Math::ifft(const OrE::Math::ComplexNumber* _pInput, int _iNum)
{
	// Alloc return memory
	OrE::Math::ComplexNumberP pResult = (OrE::Math::ComplexNumberP)malloc(_iNum * sizeof(OrE::Math::ComplexNumber));
	// Precompute all factors and set static informations
	StaticState State;
	State.iInitialStride = 1; State.iNum = _iNum; State.bInverse = true;
    Factorize(_iNum, State.aiFactors);
	ComputeFTFactors(&State.pFTFactors, _iNum, true);

	FFTKernel(pResult, _pInput, 1, State.aiFactors, State);

	free(State.pFTFactors);

	// Normalize
	float fInvLength = 1.0f/_iNum;
	for(int i=0; i<_iNum; ++i)
		pResult[i] *= fInvLength;

	return pResult;
}

// ******************************************************************************** //
OrE::Math::ComplexNumberP OrE::Math::fft2(const OrE::Math::ComplexNumber* _pInput, int _iNumX, int _iNumY)
{
	// Alloc return memory
	OrE::Math::ComplexNumberP pResult = (OrE::Math::ComplexNumberP)malloc(_iNumX * _iNumY * sizeof(OrE::Math::ComplexNumber));
	// Precompute all factors and set static informations
	StaticState State;
	State.iInitialStride = 1; State.bInverse = false;

	// Exploit separability: Transformation of X direction
	State.iNum = _iNumX;
	Factorize(_iNumX, State.aiFactors);
	ComputeFTFactors(&State.pFTFactors, _iNumX, false);

	for(int i=0; i<_iNumY; ++i)
		FFTKernel(pResult, _pInput + i*_iNumX, 1, State.aiFactors, State);

	// Recompute the static factors if not symmetric
	if(_iNumX != _iNumY)
	{
		free(State.pFTFactors);
		Factorize(_iNumY, State.aiFactors);
		ComputeFTFactors(&State.pFTFactors, _iNumY, false);
	}

	// Compute FFT in second direction
	State.iInitialStride = _iNumX;
	for(int i=0; i<_iNumY; ++i)
		FFTKernel(pResult, _pInput + i, 1, State.aiFactors, State);

	free(State.pFTFactors);

	return pResult;
}
