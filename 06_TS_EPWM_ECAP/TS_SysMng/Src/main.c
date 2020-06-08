/***********************************************************************************
 * File                     :main.c
 *
 * Title                    :
 *
 * Author                   :Tarik SEMRADE
 *
 * Description              :
 *
 * Version                  :
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
 *********************************************************************************/

/**********************************************************************************
 *  Included Files
 *
 *********************************************************************************/
#include "F28x_Project.h"
#include "device.h"
#include "main.h"
#include "gpio.h"
#include "TS_SysMng_Adc.h"
#include "TS_SysMng_ePwm.h"
#include "TS_SysMng_Dac.h"
#include "TS_SysMng_Ecap.h"
#include "TS_SysMng_Gpio.h"
/**********************************************************************************
 *  External Included Files
 *
 *********************************************************************************/

/**********************************************************************************
 *  Defines
 *
 *********************************************************************************/

/**********************************************************************************
 *  Global Variables
 *
 *********************************************************************************/

/**********************************************************************************
 * \function:       main
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void main(void)
{
    /***************************CPU Initialization*********************************/

    /* Set up system flash and turn peripheral clocks */
    InitSysCtrl();

    /* GPIO Init */
    InitGpio();

    /* Globally disable maskable CPU interrupts */
    DINT;

    /* Clear and disable all PIE interrupts */
    InitPieCtrl();

    /* Individually disable maskable CPU interrupts */
    IER = 0x0000;

    /* Clear all CPU interrupt flags */
    IFR = 0x0000;

    /* Populate the PIE interrupt vector table with */
    InitPieVectTable();

    /***********************Interrupt linking functions*****************************/

    /* Map ISR ADC_A function */
    EALLOW;
    PieVectTable.ADCA1_INT = &TS_SysMng_Adca1ISR;

    /* Map ISR ECAP1 function */
    PieVectTable.ECAP1_INT = &TS_SysMng_Ecap1ISR;
    EDIS;

    /************************Peripheral Initialization*****************************/
    /* ADC Reset */
    TS_SysMng_AdcConfig();

    /* ECAP1 Reset */
    TS_SysMng_EcapConfig();

    /* EPWM1 Reset */
    TS_SysMng_ePwm1Config();

    /* EPWM2 Reset */
    TS_SysMng_ePwm2Config();

    /* Enable EPWM clock*/
    TS_SysMng_ePwmsClockEnalbe();

    /* DAC Reset */
    TS_SysMng_DacCongif();

    /* GPIO24 Reser*/
    TS_SysMng_GpioConfig();

    /* Enable group 1 interrupts */
    IER |= M_INT1;

    /* Enable INT4 in IER to enable PIE group 4 */
    IER |= M_INT4;

    /* Enable Global interrupt INTM */
    EINT;

    /* Enable Global real-time interrupt DBGM */
    ERTM;

    /* Main loop */
    while (1)
    {
        asm(" NOP");

    }

}
