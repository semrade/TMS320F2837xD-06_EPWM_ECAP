/***********************************************************************************
 * File              :TS_SysMng_Ecap.c
 *
 * Title             :
 *
 * Author            :Tarik SEMRADE
 *
 * Created on        :Mar 21, 2020
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
#include "F28x_Project.h"
#include "TS_SysMng_Ecap.h"
#include "ecap.h"
/**********************************************************************************
 *  Global Variables
 *
 *********************************************************************************/
Uint32 u32PwmDuty;                         // measured PWM duty cycle
Uint32 u32PwmPeriod;                       // measured PWM period
/**********************************************************************************
 * \function:       TS_SysMng_EcapConfig
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_SysMng_EcapConfig(void)
{

    /* Enable EALLOW protected register access */
    EALLOW;

    /* eCAP1 is reset */
    DevCfgRegs.SOFTPRES3.bit.ECAP1 = 1;

    /* eCAP1 is released from reset */
    DevCfgRegs.SOFTPRES3.bit.ECAP1 = 0;

    EDIS;
    /* Disable all eCAP interrupts*/
    ECap1Regs.ECEINT.all = 0;

    /* Stop the counter */
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;

    /* Clear the counter */
    ECap1Regs.TSCTR = 0;

    /* Clear the counter phase register */
    ECap1Regs.CTRPHS = 0;

    /* ---------------Capture control1 Register configuration---------------*/
    /* ---------------------------------------------------------------------*/
    /* bit 15-14     11:     FREE/SOFT, 11 = ignore emulation suspend       */
    /* bit 13-9      00000:  PRESCALE, 00000 = divide by 1                  */
    /* bit 8         1:      CAPLDEN, 1 = enable capture results load       */
    /* bit 7         0:      CTRRST4, 1 = reset counter on CAP4 event       */
    /* bit 6         1:      CAP4POL, 0 = rising edge, 1 = falling edge     */
    /* bit 5         0:      CTRRST3, 0 = do not reset counter on CAP3 event*/
    /* bit 4         0:      CAP3POL, 0 = rising edge, 1 = falling edge     */
    /* bit 3         0:      CTRRST2, 0 = do not reset counter on CAP2 event*/
    /* bit 2         1:      CAP2POL, 0 = rising edge, 1 = falling edge     */
    /* bit 1         0:      CTRRST1, 0 = do not reset counter on CAP1 event*/
    /* bit 0         0:      CAP1POL, 0 = rising edge, 1 = falling edge     */
    /*----------------------------------------------------------------------*/

    ECap1Regs.ECCTL1.bit.CAP2POL = 0x1;
    ECap1Regs.ECCTL1.bit.CAP4POL = 0x1;
    ECap1Regs.ECCTL1.bit.CTRRST4 = 0x1;
    ECap1Regs.ECCTL1.bit.CAPLDEN = 0x1;
    ECap1Regs.ECCTL1.bit.PRESCALE = 0x0;
    ECap1Regs.ECCTL1.bit.FREE_SOFT = 0x3;

    /* ---------------Capture control2 Register configuration---------------*/
    /* ---------------------------------------------------------------------*/
    /* bit 15-11     00000:  reserved                                       */
    /* bit 10        0:      APWMPOL, don't care                            */
    /* bit 9         0:      CAP/APWM, 0 = capture mode, 1 = APWM mode      */
    /* bit 8         0:      SWSYNC, 0 = no action (no s/w synch)           */
    /* bit 7-6       10:     SYNCO_SEL, 10 = disable sync out signal        */
    /* bit 5         0:      SYNCI_EN, 0 = disable Sync-In                  */
    /* bit 4         1:      TSCTRSTOP, 1 = enable counter                  */
    /* bit 3         0:      RE-ARM, 0 = don't re-arm, 1 = re-arm           */
    /* bit 2-1       11:     STOP_WRAP, 11 = wrap after 4 captures          */
    /* bit 0         0:      CONT/ONESHT, 0 = continuous mode               */
    /*----------------------------------------------------------------------*/

    ECap1Regs.ECCTL2.bit.CONT_ONESHT = 0x0;
    ECap1Regs.ECCTL2.bit.STOP_WRAP = 0x3;
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0x1;
    ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0x2;

    /* --------------Capture Interrupt Register configuration---------------*/
    /* ---------------------------------------------------------------------*/
    /* bit 15-8      0's:    reserved                                       */
    /* bit 7         0:      CTR=CMP, 0 = compare interrupt disabled        */
    /* bit 6         0:      CTR=PRD, 0 = period interrupt disabled         */
    /* bit 5         0:      CTROVF, 0 = overflow interrupt disabled        */
    /* bit 4         0:      CEVT4, 0 = event 4 interrupt disabled          */
    /* bit 3         1:      CEVT3, 1 = event 3 interrupt enabled           */
    /* bit 2         0:      CEVT2, 0 = event 2 interrupt disabled          */
    /* bit 1         0:      CEVT1, 0 = event 1 interrupt disabled          */
    /* bit 0         0:      reserved                                       */
    /*----------------------------------------------------------------------*/

    /* Enable desired eCAP interrupts */
    ECap1Regs.ECEINT.bit.CEVT3 = 0x1;

    /* Enable ECAP1_INT in PIE group 4 */
    PieCtrlRegs.PIEIER4.bit.INTx1 = 1;

    /* Enable INT4 in IER to enable PIE group 4 */
    //IER |= M_INT4;

}
/**********************************************************************************
 * \function:       Ecap1_ISR
 * \brief           PIE4.1 @ 0x000D70  eCAP1 interrupt
 * \param[in]       void
 * \return          void
 **********************************************************************************/
interrupt void TS_SysMng_Ecap1ISR(void)
{
    /* Must acknowledge the PIE group */
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

    /* Clear the CEVT3 event flag */
    ECap1Regs.ECCLR.bit.CEVT3 = 1;

    /* Clear the ECAP1 interrupt flag */
    ECap1Regs.ECCLR.bit.INT = 1;

    /*
     * Note:
     * Remember that the eCAP1 is running at the full 200 MHz of the device.
     * Captured values reflect this time base.
     */

    /* Compute the PWM duty period (rising edge to falling edge) */
    u32PwmDuty = ECap1Regs.CAP2 - ECap1Regs.CAP1;

    /* Compute the PWM period (rising edge to rising edge) */
    u32PwmPeriod = ECap1Regs.CAP3 - ECap1Regs.CAP1;
}
