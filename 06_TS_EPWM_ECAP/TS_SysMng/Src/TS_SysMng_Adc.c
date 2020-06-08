/***********************************************************************************
 * File              :TS_SysMng_ADC.c
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

/**********************************************************************************
 *  Included Files
 *
 *********************************************************************************/
#include "F28x_Project.h"
#include "TS_SysMng_Adc.h"
#include "TS_SysMng_ePwm.h"
#include "TS_DspLUT_X.h"
#include "F2837xD_Adc_defines.h"
#include "adc.h"
/**********************************************************************************
 *  Global Variables
 *
 *********************************************************************************/
volatile Uint16 s_u16PwmModDelay = 0;
volatile Uint16 s_u16PwmModDir = 1;
volatile Uint16 s_u16PwmCmpaVal = TS_SYSMNG_EPWM_MAX_DUTY;
Uint16 u16DacOffset;              /* DAC offset */
Uint16 u16DacOutput;              /* DAC output */
Uint16 t_u16AdcBuf[BUFFER_SIZE];

/**********************************************************************************
 * \function:       TS_SysMng_AdcConfig
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_SysMng_AdcConfig(void)
{
    /* Enable EALLOW protected register access */
    EALLOW;

    /* ADC is reset*/
    DevCfgRegs.SOFTPRES13.bit.ADC_A = 1;

    /* ADC is released from reset */
    DevCfgRegs.SOFTPRES13.bit.ADC_A = 0;

    /* Interrupt pulse position when end of conversion occurs */
    AdcaRegs.ADCCTL1.all = ADC_PULSE_END_OF_CONV;

    /* Adc clock configuration is 50Khz */
    AdcaRegs.ADCCTL2.bit.PRESCALE = ADC_CLK_DIV_4_0;

    /* Call AdcSetMode() to configure the resolution and signal mode. */
    /* This also performs the correct ADC calibration for the configured mode. */
    AdcSetMode(ADC_ADCA, ADC_BITRESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    /*
     * => SOC0 configuration
     * => Trigger using ePWM2-ADCSOCA
     * => Convert channel ADCINA0 (Ch. 0)
     * => Acquisition window set to (19+1)=20 cycles (100 ns with 200 MHz SYSCLK)
     * */
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 19;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = ADC_CH_ADCIN0;
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM2_SOCA;

    /* No ADC interrupt triggers SOC0 (TRIGSEL field determines trigger) */
    AdcaRegs.ADCINTSOCSEL1.bit.SOC0 = ADC_INT_SOC_TRIGGER_NONE;

    /* All SOCs handled in round-robin mode, no priority */
    AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = ADC_PRI_ALL_ROUND_ROBIN;

    /* => ADCA1 interrupt configuration
     * => Interrupt pulses regardless of flag state
     * => Enable the interrupt in the ADC
     * => EOC0 triggers the interrupt
     * */
    AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 0x1;
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = ADC_SOC_NUMBER0;
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = ADC_INT_SOC_TRIGGER_ADCINT1;

    /*
     * => Enable ADCA1 interrupt in PIE group 1
     * => Enable INT1 in IER to enable PIE group
     * */
    PieCtrlRegs.PIEIER1.bit.INTx1 = 0x1;
//    IER |= M_INT1;

    /*
     * => Power up the ADC
     * => Wait 1 ms after power-up before using the ADC
     * => Disable EALLOW protected register access
     * */
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 0x1;
    DELAY_US(1000);
    EDIS;

}
/**********************************************************************************
 * \function:       Adca1_ISR
 * \brief           PIE1.1 @0x000D40  ADC-A interrupt #1
 * \param[in]       void
 * \return          void
 **********************************************************************************/
interrupt void TS_SysMng_Adca1ISR(void)
{
    Uint16 *s_pu16AdcBufPtr = t_u16AdcBuf;
    static Uint16 s_iqSinusTableIndexer; /* set to 0 by the compiler */

    /************************Store ADC result into tab*****************************/
    /*  Must acknowledge the PIE group */
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    /* Clear ADCINT1 flag */
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    /* Read the ADC result */
    *s_pu16AdcBufPtr++ = AdcaResultRegs.ADCRESULT0;

    /* Brute-force the circular buffer */
    if (s_pu16AdcBufPtr == (t_u16AdcBuf + BUFFER_SIZE))
    {
        /* Rewind the pointer to beginning */
        s_pu16AdcBufPtr = t_u16AdcBuf;
    }

    /*************Generate a sin wave to feed ADC in*******************************/

    if (1 == 1)
    {
//        u16DacOutput =
//                u16DacOffset
//                        + ((i16LutSinQuadrature[s_iqSinusTableIndexer++]
//                                ^ 0x8000) >> 5);
    }
    else
    {
        u16DacOutput = u16DacOffset;
    }

    /* Wrap the index */
    if (s_iqSinusTableIndexer > (TS_DSPDATALOG_SINE_LUT_SIZE - 1))
    {
        s_iqSinusTableIndexer = 0;
    }

    /* Write value to dac */
    DacbRegs.DACVALS.all = u16DacOutput;

    /* ----Modulate ePWM1A output between 10% and 90% duty cycle---------- */
    /* Change PWM slowly to see waveform */
    if (s_u16PwmModDelay++ >= 100)
    {
        /* Modulate PWM waveform */
        if (s_u16PwmModDir == 0)
        {
            /* Increment CMPA value by step size */
            s_u16PwmCmpaVal += TS_SYSMNG_EPWM_STEP;

            /* If limit reached then change direction */
            if (s_u16PwmCmpaVal >= TS_SYSMNG_EPWM_MIN_DUTY)
            {
                /* Set to minimum duty cycle */
                s_u16PwmCmpaVal = TS_SYSMNG_EPWM_MIN_DUTY;

                /* Reverse direction */
                s_u16PwmModDir = 1;
            }
        }
        else
        {
            /* Decrement CMPA value by step size */
            s_u16PwmCmpaVal -= TS_SYSMNG_EPWM_STEP;

            /* If limit reached then change direction */
            if (s_u16PwmCmpaVal <= TS_SYSMNG_EPWM_MAX_DUTY)
            {
                /* Set to maximum duty cycle */
                s_u16PwmCmpaVal = TS_SYSMNG_EPWM_MAX_DUTY;

                /* Reverse direction */
                s_u16PwmModDir = 0;
            }
        }

        /* Updated PWM CMPA value */
        EPwm1Regs.CMPA.bit.CMPA = s_u16PwmCmpaVal;

        /* Reset the delay counter */
        s_u16PwmModDelay = 0;
    }

}

