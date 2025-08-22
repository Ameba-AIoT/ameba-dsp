/* ------------------------------------------------------------------------ */
/* Copyright (c) 2020-2025 Cadence Design Systems, Inc. ALL RIGHTS RESERVED.*/
/* These coded instructions, statements, and computer programs ('Cadence    */
/* Libraries') are the copyrighted works of Cadence Design Systems Inc.     */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence license.                                      */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* NatureDSP Signal Library for HiFi 5s DSP                                  */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (c) 2009-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef __NATUREDSP_SIGNAL_COMPLEX_H__
#define __NATUREDSP_SIGNAL_COMPLEX_H__

#include "NatureDSP_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
  Complex Mathematics:
  vec_complex2mag, vec_complex2invmag   Complex Magnitude
===========================================================================*/

/*-------------------------------------------------------------------------
  Complex magnitude
  Routines compute complex magnitude or its reciprocal

  Precision: 
  f     single precision floating point

  Input:
  x[N]  input complex data
  N     length of vector
  Output:
  y[N]  output data

  Restriction:
  none
-------------------------------------------------------------------------*/
void       vec_complex2mag    (float32_t  * y, const complex_float  * x, int N);
void       vec_complex2invmag (float32_t  * y, const complex_float  * x, int N);
float32_t  scl_complex2mag    (complex_float x);
float32_t  scl_complex2invmag (complex_float x);
void		cxfir_Freq_acorrf( complex_float *  r,const complex_float *x,int N);
void		cxfir_FreqXcorrf(complex_float * r, const complex_float * x, const complex_float * y, int N);
void		vec_cplx_Coherencef(complex_float* ccohy, complex_float*  x0, complex_float*  x1, int N);
void 		vec_cplx_Conjf(complex_float * r, const complex_float * x, int N);
void 		vec_cplx_Normf(complex_float* cny, complex_float* restrict x, int N);
void 		vec_cplx_SqNormf(float32_t* csny, complex_float* restrict x, int N);
void 		vec_cplx_WeightedSumfV1(complex_float*  cwsy, complex_float*  x1, complex_float*  x2,
							 complex_float*  w1, complex_float*  w2, int N);
void 		vec_cplx_WeightedSumfV2(complex_float* cwsy, complex_float*  x0,
							 complex_float*  x1, float32_t*  w, int N );
void 		vec_cplx_WeightedSumfV3(complex_float*  cwsy, complex_float*  x0, complex_float*  x1,
									complex_float*  w,  int N);
void 		vec_cplx_WeightedSum_hmfV1(complex_float*  cwsy, complex_float*  x1, complex_float*  x2,
							 complex_float*  w1, complex_float*  w2, int N);
void 		vec_cplx_WeightedSum_hmfV2(complex_float* cwsy, complex_float*  x0,
							 complex_float*  x1, float32_t*  w, int N );
void 		vec_cplx_WeightedSum_hmfV3(complex_float*  cwsy, complex_float*  x0, complex_float*  x1,
									complex_float*  w,  int N);
void 		cxfir_Freq_acorrhf(complex_float16 * r,const complex_float16 * x, int N);
void 		cxfir_FreqXcorrhf(complex_float16 * r, const complex_float16 * x, const complex_float16 * y, int N);
void 		vec_cplx_Coherencehf(complex_float16* ccohy, complex_float16* x0, complex_float16* x1, int N);
void 		vec_cplx_conjhf(complex_float16 * y, const complex_float16 *  x, int N);
void 		vec_cplx_Normhf(complex_float16 * cny, complex_float16* x, int N);
void 		vec_cplx_SqNormhf(float16_t* csny, complex_float16* x, int N);
void 		vec_cplx_WeightedSumhfV1(complex_float16* y, complex_float16* x0, complex_float16* x1, complex_float16* w0, complex_float16* w1, int N);
void 		vec_cplx_WeightedSumhfV2(complex_float16* cwsy, complex_float16* x0, complex_float16* x1, float16_t* w, int N );
void 		vec_cplx_WeightedSumhfV3(complex_float16* cwsy, complex_float16* x0, complex_float16* x1,complex_float16* w, int N );
void 		vec_cplx_WeightedSum_hmhfV1(complex_float16* y, complex_float16* x0, complex_float16* x1, complex_float16* w0, complex_float16* w1, int N);
void 		vec_cplx_WeightedSum_hmhfV2(complex_float16* cwsy, complex_float16* x0, complex_float16* x1, float16_t* w, int N );
void 		vec_cplx_WeightedSum_hmhfV3(complex_float16* cwsy, complex_float16* x0, complex_float16* x1,complex_float16* w, int N );

#ifdef __cplusplus

}
#endif

#endif/* __NATUREDSP_SIGNAL_COMPLEX_H__ */
