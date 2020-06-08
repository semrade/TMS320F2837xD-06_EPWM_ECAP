/***********************************************************************************
 * File              :TS_SysMng_Gpio.c
 *
 * Title             :
 *
 * Author            :Tarik SEMRADE
 *
 * Created on        :Mar 22, 2020
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
/**********************************************************************************
 *  Included Files
 *
 *********************************************************************************/
#include "TS_SysMng_Gpio.h"
#include "F28x_Project.h"
/**********************************************************************************
 * \function:       TS_SysMng_GpioConfig
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_SysMng_GpioConfig(void)
{
    /* Enable EALLOW protected register access */
    EALLOW;

    /* Inpux Xbar configuration
     * Feed GPIO24 to eCAP1 via Input X-Bar
     * */
    InputXbarRegs.INPUT7SELECT = 24;

    /* Gpio 24 Configuration for Ecap1 input
     * Group A pins
     * */
    GpioCtrlRegs.GPACTRL.all    = 0x00000000; /* QUALPRD = PLLSYSCLK for all group A GPIO                   */
    GpioCtrlRegs.GPAQSEL1.all   = 0x00000000; /* Synchronous qualification for all group A GPIO 0-15        */
    GpioCtrlRegs.GPAQSEL2.all   = 0x00000000; /* Synchronous qualification for all group A GPIO 16-31       */
    GpioCtrlRegs.GPADIR.all     = 0x00000000; /* All GPIO are inputs                                        */
    GpioCtrlRegs.GPAPUD.all     = 0x00000000; /* All pullups enabled                                        */
    GpioCtrlRegs.GPAINV.all     = 0x00000000; /* No inputs inverted                                         */
    GpioCtrlRegs.GPAODR.all     = 0x00000000; /* All outputs normal mode (no open-drain outputs)            */
    GpioCtrlRegs.GPACSEL1.all   = 0x00000000; /* GPIO 0-7   \.                                              */
    GpioCtrlRegs.GPACSEL2.all   = 0x00000000; /* GPIO 8-15   |. GPIODAT/SET/CLEAR/TOGGLE reg. master:       */
    GpioCtrlRegs.GPACSEL3.all   = 0x00000000; /* GPIO 16-23  |. 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA1    */
    GpioCtrlRegs.GPACSEL4.all   = 0x00000000; /* GPIO 24-31 /.                                              */
    /********************************************************************************************************/
    /* GPIO 0 for EPWM1 output */
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;        // 0|0=GPIO  0|1=EPWM1A       0|2=rsvd         0|3=rsvd
    GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 1;        // 1|0=GPIO  1|1=rsvd         1|2=SDAA         1|3=rsvd
                                                // 2|0=GPIO  2|1=rsvd         2|2=rsvd         2|3=rsvd
                                                // 3|0=GPIO  3|1=rsvd         3|2=rsvd         3|3=rsvd

    /* GPIO 24 for ECAP1 in                 GPyGMUXn|GPyMUXn n(1,2) and    X1(0..15),X2(16..31)             */
    GpioCtrlRegs.GPAGMUX2.bit.GPIO24 = 0;       // 0|0=GPIO  0|1=OUTPUTXBAR1  0|2=EQEP2A       0|3=MDXB
    GpioCtrlRegs.GPAMUX2.bit.GPIO24  = 0;       // 1|0=GPIO  1|1=rsvd         1|2=SPISIMOB     1|3=SD2_D1
                                                // 2|0=GPIO  2|1=rsvd         2|2=rsvd         2|3=rsvd
                                                // 3|0=GPIO  3|1=rsvd         3|2=rsvd         3|3=rsvd

    /* Disable EALLOW protected register access */
    EDIS;
}
