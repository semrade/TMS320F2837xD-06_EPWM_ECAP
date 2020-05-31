/***********************************************************************************
 * File              :TS_SysMng_ePwm.c
 *
 * Title             :
 *
 * Author            :Tarik SEMRADE
 *
 * Created on        :Mar 19, 2020
 *
 * Version           :
 *
 * Description       :
 *
 * Copyright (c) 2020 Tarik SEMRADE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************/
#include "TS_DspLUT.h"						// Main include file
#include "F2837xD_device.h"


/**********************************************************************
*  Sine table for use with DAC
**********************************************************************/

// quadrature look-up table: contains 4 quadrants of sinusoid data points
int16 i16LutSinQuadrature[TS_DSPDATALOG_SINE_LUT_SIZE] = {
		0x0000,			// [0]  0.0
		0x1FD4,	        // [1]  14.4
		0x3DA9,	        // [2]  28.8
		0x579E,	        // [3]  43.2
		0x6C12,	        // [4]  57.6
		0x79BB,	        // [5]  72.0
		0x7FBE,	        // [6]  86.4
		0x7DBA,	        // [7]  100.8
		0x73D0,	        // [8]  115.2
		0x629F,	        // [9]  129.6
		0x4B3B,	        // [10] 144.0
		0x2F1E,	        // [11] 158.4
		0x100A,	        // [12] 172.8
		0xEFF6,	        // [13] 187.2
		0xD0E2,	        // [14] 201.6
		0xB4C5,	        // [15] 216.0
		0x9D61,	        // [16] 230.4
		0x8C30,	        // [17] 244.8
		0x8246,	        // [18] 259.2
		0x8042,	        // [19] 273.6
		0x8645,        	// [20] 288.0
		0x93EE,	        // [21] 302.4
		0xA862,	        // [22] 316.8
		0xC257,	        // [23] 331.2
		0xE02C         	// [24] 345.6
		};

