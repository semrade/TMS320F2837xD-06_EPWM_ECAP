/***********************************************************************************
 * File              :TS_SysMng_ePwm.h
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
#ifndef TS_SYSMNG_INC_TS_SYSMNG_EPWM_H_
#define TS_SYSMNG_INC_TS_SYSMNG_EPWM_H_

/**********************************************************************************
 *  Defines
 *
 *********************************************************************************/

#define TS_SYSMNG_EPMW_SAMPLE_PERIOD   1999            /* 1999 = 50 kHz sampling w/ 100 MHz ePWM clock */
#define TS_SYSMNG_EPMW_PHASE           0x0000
#define TS_SYSMNG_EPMW_COUNT_CLEAR     0x0000
#define TS_SYSMNG_EPMW_HALF_PERIOD     25000           /* period/2 for 2 kHz symmetric PWM w/ 100 MHz ePWM clock */
#define TS_SYSMNG_EPWM_DUTY_CYCLE      18750           /* 25% duty cycle */
#define TS_SYSMNG_EPWM_MIN_DUTY        22500           /* 10% duty cycle for PWM modulation */
#define TS_SYSMNG_EPWM_MAX_DUTY        2500            /* 90% duty cycle for PWM modulation */
#define TS_SYSMNG_EPWM_STEP            10              // Step size change for PWM modulation

/***********************************************************************************
 * Function prototypes
 *
 ***********************************************************************************/
void TS_SysMng_ePwm1Config (void);
void TS_SysMng_ePwm2Config (void);
void TS_SysMng_ePwmClockEnalbe(void);
void TS_SysMng_Protection(void);



#endif /* TS_SYSMNG_INC_TS_SYSMNG_EPWM_H_ */
