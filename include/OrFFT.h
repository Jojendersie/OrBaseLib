// ******************************************************************************** //
// OrFFT.h																			//
// =======																			//
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

namespace OrE {
namespace Math {

// ******************************************************************************** //
// 1D Fourier transformations
ComplexNumberP fft(const float* _pfInput, int _iNum);	// slower scince copy intern!!!
ComplexNumberP fft(const OrE::Math::ComplexNumber* _pInput, int _iNum);
ComplexNumberP ifft(const OrE::Math::ComplexNumber* _pInput, int _iNum);

// 2D Fourier transformations
ComplexNumberP fft2(const OrE::Math::ComplexNumber* _pInput, int _iNumX, int _iNumY);

}; // namespace Math
}; // namespace OrE
// *************************************EOF**************************************** //