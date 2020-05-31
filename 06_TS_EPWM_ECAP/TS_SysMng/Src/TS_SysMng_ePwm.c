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
#include "TS_SysMng_ePwm.h"
#include "F28x_Project.h"
#include "F2837xD_EPwm_defines.h"
#include "hw_epwm.h"
#include "epwm.h"
/**********************************************************************************
 * \function:       TS_SysMng_ePwm2Config
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_SysMng_ePwm2Config(void)
{
    /* Enable EALLOW protected register access */
    EALLOW;

    /* Configure the prescaler to the ePWM modules.  Max ePWM input clock is 100 MHz. */
    /* EPWMCLK divider from PLLSYSCLK  1=/2 => 200MHz/2 = 100MHz */
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 1;

    /* Must disable the clock to the ePWM modules if you want all ePWM modules
     * synchronized. */
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;

    /* Disable EALLOW protected register access */
    EDIS;

    /* Enable EALLOW protected register access */
    EALLOW;

    /* ePWM2 is reset */
    DevCfgRegs.SOFTPRES2.bit.EPWM2 = 1;

    /* ePWM2 is released from reset */
    DevCfgRegs.SOFTPRES2.bit.EPWM2 = 0;

    /* Disable EALLOW protected register access */
    EDIS;

    /* Freeze counter */
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_FREEZE;

    /* Phase control disabled*/
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;

    /* Reload PRD on TBCTR = 0 */
    EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;

    /* Sync-out disabled */
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;

    /* No software forced sync produced*/
    EPwm2Regs.TBCTL.bit.SWFSYNC = 0;

    /* HSPCLK = EPWMCLK/2 => 100Mhz/1 = 100MHz */
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;

    /* BCLK = HSPCLK/1 */
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    /* Counter down after sync event */
    EPwm2Regs.TBCTL.bit.PHSDIR = TB_DOWN;

    /* No emulation suspend, Free run */
    EPwm2Regs.TBCTL.bit.FREE_SOFT = 3;

    /* Clear timer counter */
    EPwm2Regs.TBCTR = TS_SYSMNG_EPMW_COUNT_CLEAR;

    /*------------------------------------------------------------------*/
    /*---- Configure ePWM2 to trigger ADC SOCA at a 50 kHz rate-------- */
    /*---------------- x * 50 Khz = 100Mhz ==> x = 2000-----------------*/
    /*------------------------------------------------------------------*/

    /* Set timer period */
    EPwm2Regs.TBPRD = TS_SYSMNG_EPMW_SAMPLE_PERIOD;

    /* Set timer phase */
    EPwm2Regs.TBPHS.bit.TBPHS = TS_SYSMNG_EPMW_PHASE;

    /* SOCA Configure */
    /* Generate SOCA on first event and keep the reset values for the rest
     * of the ETPS Register */
    EPwm2Regs.ETPS.bit.SOCAPRD = ET_1ST;

    /* Enable SOCA to ADC  */
    EPwm2Regs.ETSEL.bit.SOCASEL = 2;

    /* Enable SOCA on PRD counter */
    EPwm2Regs.ETSEL.bit.SOCAEN = 0x1;

    /* Enable TB on count up mode */
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;

}
/**********************************************************************************
 * \function:       TS_SysMng_ePwm1Config
 * \brief           Configure ePWM1 for 2 kHz symmetric PWM on EPWM1A pin
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_SysMng_ePwm1Config(void)
{

    /* Enable EALLOW protected register access */
    EALLOW;

    /* ePWM1 is reset */
    DevCfgRegs.SOFTPRES2.bit.EPWM1 = 1;

    /* ePWM1 is released from reset */
    DevCfgRegs.SOFTPRES2.bit.EPWM1 = 0;

    /*------------------------------Time Base (TB)----------------------------*/
    /**************************************************************************
     * Step 1: Scale the time-base clock (TBCLK) relative to the ePWM
     *         clock (EPWMCLK).
     **************************************************************************/

    /* Configure the prescaler to the ePWM modules.  Max ePWM input clock is 100 MHz. */
    /* EPWMCLK divider from PLLSYSCLK  1=/2 => 200MHz/2 = 100MHz */
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 1;

    /* Must disable the clock to the ePWM modules if you want all ePWM modules
     * synchronized. */
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;

    /* HSPCLK = EPWMCLK/2 => 100Mhz/1 = 100MHz */
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;

    /* BCLK = HSPCLK/1 */
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    /* FREEZE the timer */
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_FREEZE;

    /**************************************************************************
     * Step 2: Configure the PWM time-base counter (TBCTR) frequency or period.
     *************************************************************************/

    /*************************************************************************/
    /*-------- Configure ePWM1 for 2 kHz symmetric PWM on EPWM1A pin---------*/
    /*************************************************************************/

    /* Set timer period */
    EPwm1Regs.TBPRD = TS_SYSMNG_EPMW_HALF_PERIOD;

    /* Clear timer counter */
    EPwm1Regs.TBCTR = TS_SYSMNG_EPMW_COUNT_CLEAR;

    /****************************************************************************
     * Step 3: Set the mode for the time-base counter
     ****************************************************************************/

    /* Enable TB on count up and down mode */
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;

    /*****************************************************************************
     * Step 4: Configure the time-base phase relative to another ePWM module.
     *****************************************************************************/

    /* EXTSYNCIN1 */

    /*****************************************************************************
     * Step 5: Synchronize the time-base counter between modules through hardware
     *         or software.
     *****************************************************************************/
    /* Phase control disabled*/
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;

    /* No software forced sync produced*/
    EPwm1Regs.TBCTL.bit.SWFSYNC = 0;

    /* Set timer phase */
    EPwm1Regs.TBPHS.bit.TBPHS = TS_SYSMNG_EPMW_PHASE;

    /* Counter down after sync event */
    EPwm1Regs.TBCTL.bit.PHSDIR = TB_DOWN;

    /*****************************************************************************
     * Step 6: Configure how the time-base counter will behave when the device is
     *         halted by an emulator.
     *****************************************************************************/

    /* No emulation suspend, Free run, non stop */
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 3;

    /******************************************************************************
     * Step 7: Specify the source for the synchronization output of the ePWM module.
     ******************************************************************************/

    /* Sync-out disabled */
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;

    /* Disable Sync via another ePWM */
    EPwm1Regs.TBCTL2.bit.SYNCOSELX = 0x0;

    /******************************************************************************
     * Step 8: Configure one shot and global load of registers in this module.
     ******************************************************************************/

    /* Period shadow */
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;

    /* Reload PRD on TBCTR = 0 */
    EPwm1Regs.TBCTL2.bit.PRDLDSYNC = CC_CTR_ZERO;

    /* Disabled EPWMxSYNCO */
    EPwm1Regs.TBCTL2.bit.SYNCOSELX = 0x0;

    /* One-shot sync disabled */
    EPwm1Regs.TBCTL2.bit.OSHTSYNCMODE = 0x0;

    /*-----------------------Counter Compare (CC)---------------------------------*/
    /******************************************************************************
     * Step 1: Specify the PWM duty cycle for output EPWMxA and/or output EPWMxB
     ******************************************************************************/

    /* Set PWM duty cycle */
    EPwm1Regs.CMPA.bit.CMPA = TS_SYSMNG_EPWM_DUTY_CYCLE;

    /******************************************************************************
     * Step 2: Specify the time at which switching events occur on
     *         the EPWMxA or EPWMxB output
     *****************************************************************************/

    /* Load on either CTR = Zero or CTR = PRD */
    EPwm1Regs.CMPCTL.bit.LOADAMODE = 0x2;

    /* Compare control load mode */
    EPwm1Regs.CMPCTL.bit.LOADASYNC = 0x0;

    /******************************************************************************
     * Step 3: Specify the programmable delay for interrupt and SOC generation
     *         with additional comparators
     ******************************************************************************/
    /* No interrupt is needed to be generated */

    /******************************************************************************
     * Step 4: Simultaneous writes to the CMPA, CMPB, CMPC, CMPD registers on
     *         all PWM's corresponding to the configuration on EPWMXLINK.
     ******************************************************************************/
    /* No EPWM linking */

    /*-----------------------Action Qualifier (AQ)-------------------------------*/
    /******************************************************************************
     * Step 1: Specify the type of action taken when a time-base counter-compare,
     *           trip-zone submodule, or comparator event occurs
     *****************************************************************************/

    /* Clear force EPWMxA output low */
    EPwm1Regs.AQCTLA.bit.CAU = 0x1;

    /* Set force EPWMxA output high */
    EPwm1Regs.AQCTLA.bit.CAD = 0x2;

    /* Action-qualifier control register B */
    //TODO
    /* Action-qualifier control register C */
    //TODO
    /* Action-qualifier control register D */
    //TODO
    /*****************************************************************************
     * Step 2: Force the PWM output state through software control
     *****************************************************************************/

    /* No software forced EPWM output is needed */
    /* Action-qualifier s/w force register */
    EPwm1Regs.AQSFRC.all = 0x0000;

    /* Action-qualifier continuous s/w force register */
    EPwm1Regs.AQCSFRC.all = 0x0000;

    /*-----------------------------Dead Band (DB)--------------------------------*/
    /*****************************************************************************
     * Step 1: Control of traditional complementary dead-band relationship between
     *         upper and lower switches
     *****************************************************************************/

    /*****************************************************************************
     * Step 2: Specify the output rising-edge-delay value
     *         Specify the output falling-edge delay value
     *****************************************************************************/

    /*****************************************************************************
     * Step 3: Bypass the dead-band module entirely. In this case the PWM waveform
     *         is passed through without modification.
     *****************************************************************************/

    /* Dead-band disabled */
    EPwm1Regs.DBCTL.bit.OUT_MODE = 0;

    /*****************************************************************************
     * Step 4: Option to enable half-cycle clocking for double resolution
     *****************************************************************************/

    /*****************************************************************************
     * Step 5: Allow ePWMxB phase shifting with respect to the ePWMxA output
     *****************************************************************************/

    /*****************************************************************************
     * Step 6: Configure one shot and global load of registers in this module
     *****************************************************************************/

    /*-----------------------------PWM Chopper (PC)-------------------------------*/
    /******************************************************************************
     * Step 1: Create a chopping (carrier) frequency
     ******************************************************************************/

    /******************************************************************************
     * Step 2: Pulse width of the first pulse in the chopped pulse train
     ******************************************************************************/

    /******************************************************************************
     * Step 3: Duty cycle of the second and subsequent pulses
     ******************************************************************************/

    /******************************************************************************
     * Step 4: Bypass the PWM chopper module entirely. In this case the PWM
     *         waveform is passed through without modification.
     ******************************************************************************/

    /* PWM chopper unit disabled */
    EPwm1Regs.PCCTL.bit.CHPEN = 0;

    /*-------------------------------Trip Zone (TZ)-------------------------------*/
    /******************************************************************************
     * Step 1: Configure the ePWM module to react to one, all, or none of the
     *         trip-zone signals or digital compare events.
     ******************************************************************************/

    /******************************************************************************
     *Step 2: Specify the trip action taken when a fault occurs:
     *          – Force EPWMxA and/or EPWMxB high
     *          – Force EPWMxA and/or EPWMxB low
     *          – Force EPWMxA and/or EPWMxB to a high-impedance state
     *          – Configure EPWMxA and/or EPWMxB to ignore any trip condition.
     ******************************************************************************/

    /******************************************************************************
     * Step 3: Configure how often the ePWM will react to each trip-zone signal:
     *          – One-shot
     *          – Cycle-by-cycle
     ******************************************************************************/

    /******************************************************************************
     * Step 4: Enable the trip-zone to initiate an interrupt
     ******************************************************************************/

    /******************************************************************************
     * Step 5: Bypass the trip-zone module entirely.
     ******************************************************************************/
    /* All trip zone and DC compare actions disabled */
    EPwm1Regs.TZDCSEL.all = 0x0000;

    /*---------------------------Event Trigger (ET)-------------------------------*/
    /******************************************************************************
     * Step 1: Enable the ePWM events that will trigger an interrupt
     ******************************************************************************/

    /******************************************************************************
     * Step 2: Enable ePWM events that will trigger an ADC start-of-conversion event
     ******************************************************************************/

    /******************************************************************************
     * Step 3: Specify the rate at which events cause triggers (every occurrence
     *          or every 2nd or up to 15th occurrence)
     ******************************************************************************/

    /******************************************************************************
     * Step 4: Poll, set, or clear event flags
     ******************************************************************************/

    /*----------------------------Digital Compare (DC)----------------------------*/
    /******************************************************************************
     * Step 1: Enables comparator (COMP) module outputs and trip zone signals
     *         which are configured using the Input X-BAR to create events and
     *         filtered events
     ******************************************************************************/

    /******************************************************************************
     * Step 1: Specify event-filtering options to capture TBCTR counter, generate
     *         blanking window, or insert delay in PWM output or time-base counter
     *         based on captured value.
     ******************************************************************************/

    /* Disable EALLOW protected register access */
    EDIS;

}
/**********************************************************************************
 * \function:       TS_SysMng_ePwmClockEnalbe
 * \brief           Enable the clocks to the ePWM module
 *                  This should be done after all ePWM modules are configured
 *                  to ensure synchronization between the ePWM modules
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_SysMng_ePwmClockEnalbe(void)
{
    /*---------------------------------------------------------------------*/
    /*---------------- Enable the clocks to the ePWM module----------------*/
    /*---Note: this should be done after all ePWM modules are configured---*/
    /*-------to ensure synchronization between the ePWM modules------------*/
    /*---------------------------------------------------------------------*/

    /* Enable EALLOW protected register access */
    EALLOW;

    /* TBCLK to ePWM modules enabled */
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;

    /* Disable EALLOW protected register access */
    EDIS;
}
/**********************************************************************************
 * \function:       TS_SysMng_ePWMProtection
 * \brief
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_SysMng_ePWMProtection(void)
{

    EALLOW;

// Configure Trip Mechanism for the Motor control software
// -Cycle by cycle trip on CPU halt
// -One shot IPM trip zone trip
// These trips need to be repeated for EPWM1 ,2 & 3

//===========================================================================
//Motor Control Trip Config, EPwm1,2,3
//===========================================================================

// CPU Halt Trip
    EPwm1Regs.TZSEL.bit.CBC6 = 0x1;
    EPwm2Regs.TZSEL.bit.CBC6 = 0x1;
    EPwm3Regs.TZSEL.bit.CBC6 = 0x1;

    EPwm1Regs.TZSEL.bit.OSHT1 = 1;  //enable TZ1 for OSHT
    EPwm2Regs.TZSEL.bit.OSHT1 = 1;  //enable TZ1 for OSHT
    EPwm3Regs.TZSEL.bit.OSHT1 = 1;  //enable TZ1 for OSHT

// What do we want the OST/CBC events to do?
// TZA events can force EPWMxA
// TZB events can force EPWMxB

    EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
    EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

    EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
    EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

    EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
    EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

    EDIS;

    // Clear any spurious OV trip
    EPwm1Regs.TZCLR.bit.OST = 1;
    EPwm2Regs.TZCLR.bit.OST = 1;
    EPwm3Regs.TZCLR.bit.OST = 1;

//************************** End of Prot. Conf. ***************************//
}
