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

/*
    tables for sin(pi/2*x) approximation
*/
#ifndef SINFTBL_H__
#define SINFTBL_H__

#include "NatureDSP_types.h"
#include "common.h"

externC const int16_t sinhf_maxval; /* domain ranges - zero outside */
externC const union ufloat16uint16 ALIGN(8) polysinhf_tbl[];

/* 
   polynomial coefficients for cos(x), [-pi/4...pi/4]
   derived by MATLAB code:
   s=pow2(1,-16); x=(s:s:pi/4); x=[-x(end:-1:1) x];
   y=cos(x);
   p=polyfit(x,y,6); p=p(1:2:end); p(end)=[];
*/
externC const union ufloat16uint16 ALIGN(8) polycoshf_tbl[];

#endif /* SINFTBL_H__ */
