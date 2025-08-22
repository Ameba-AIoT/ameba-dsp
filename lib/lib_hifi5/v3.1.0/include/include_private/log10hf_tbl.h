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
    tables for log10hf(x) approximation
*/
#include "NatureDSP_types.h"
#include "common.h"
#define LOGHF10_TBL_ORDER 9
const union ufloat16uint16 ALIGN(16) log10hf_tbl[LOGHF10_TBL_ORDER] =
{
    {0xa8cd},/*-3.7492107494e-002*/
    {0xabe4},/*-6.1653605702e-002*/
    {0xac25},/*-6.4747533825e-002*/
    {0xac9c},/*-7.2019999908e-002*/
    {0xad8c},/*-8.6698210732e-002*/
    {0xaef3},/*-1.0857951025e-001*/
    {0xb0a2},/*-1.4476771442e-001*/
    {0xb2f3},/*-2.1714721664e-001*/
    {0xb6f3} /*-4.3429447406e-001*/
};

const union ufloat16uint16 ALIGN(16) log10hfConstants_tbl[] =
{
	{0x3288},  // 4log10_2-1
	{0x39a8},  //sqrt(0.5)
	{0x0000},  // constant 0
	{0x4000},  //constant 2
	{0x3c00},  //constant 1
	{0x7c00},  //+inf
	{0x7e00},  //qNaN
	{0xfc00},  //-Inf
};
