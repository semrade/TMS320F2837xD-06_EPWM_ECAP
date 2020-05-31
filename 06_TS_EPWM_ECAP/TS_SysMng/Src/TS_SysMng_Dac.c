/***********************************************************************************
 * File              :TS_SysMng_Dac.c
 *
 * Title             :
 *
 * Author            :Tarik SEMRADE
 *
 * Created on        :Mar 20, 2020
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
#include "F28x_Project.h"
#include "dac.h"

/**********************************************************************************
 * \function:       TS_SysMng_DacCongif
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_SysMng_DacCongif(void)
{
    /* Enable EALLOW protected register access */
    EALLOW;

    /* Configure DAC-B control registers */
    DacbRegs.DACCTL.bit.DACREFSEL = DAC_REF_ADC_VREFHI;

    /* Set DAC-B output to mid-range, DACVALS = bits 11-0 */
    DacbRegs.DACVALS.all = 0x0800;

    /* Enable DAC-B output*/
    DacbRegs.DACOUTEN.bit.DACOUTEN = 1;

    /* DAC-B lock control register */
    DacbRegs.DACLOCK.all = 0x0000; // Write a 1 to lock (cannot be cleared once set)

    /* Disable EALLOW protected register access */
    EDIS;

}

