/**
* \file main.c
*
* \brief Main source file.
*
    (c) 2019 Microchip Technology Inc. and its subsidiaries.
    
    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t signal_frequency = 0;

void CLK_Init (void);
void PORT_Init (void);
void ZCD0_Init (void);
void TCB0_Init (void);
void EVSYS_Init (void);

void CLK_Init (void)
{
    _PROTECTED_WRITE (CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSCHF_gc);  /* Select internal HF oscillator */
    _PROTECTED_WRITE (CLKCTRL.OSCHFCTRLA, CLKCTRL_FREQSEL_4M_gc);    /* Set internal clock to 4Mhz */
}

void PORT_Init (void)
{
    /* Disable digital input buffer and pull-up resistor on PD1*/
    PORTD.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
    
    /* SET PA7 as an output*/
    PORTA.OUTSET |= PIN7_bm;
    PORTA.DIRSET |= PIN7_bm;    
}

/* Initialization of the Zero Cross Detector */
void ZCD0_Init (void)
{
    ZCD0.CTRLA = ZCD_ENABLE_bm    /* Enable the ZCD*/
               | ZCD_OUTEN_bm;    /*Enable the output of the ZCD*/
}

/*Initialize TCB in pulse width-frequency measurement mode, input from ZCD through Event System */
void TCB0_Init (void)
{
    TCB0.CTRLB = TCB_CNTMODE_FRQ_gc;    /* Input Capture Frequency */
    TCB0.EVCTRL = TCB_CAPTEI_bm;        /* Event Input Enable: enabled */    
    TCB0.INTCTRL = TCB_CAPT_bm;            /* Capture or Timeout: enabled */

    TCB0.CTRLA = TCB_CLKSEL_DIV2_gc        /* CLK_PER/2 (From Prescaler) - This is needed to be able to count to 40Hz with a 4Mhz system clock */
               | TCB_ENABLE_bm            /* Enable: enabled */
               | TCB_RUNSTDBY_bm;        /* Run Standby: enabled */
}

/* Enable event generation from ZCD to TCB */
void EVSYS_Init (void)
{
    EVSYS.CHANNEL0 = EVSYS_CHANNEL0_ZCD0_gc;  /* Zero-cross detector 0 out linked to Event Channel 0 */
    EVSYS.USERTCB0CAPT = EVSYS_CHANNEL00_bm;  /* TCB uses Event Channel 0 */
}

int main(void)
{
    CLK_Init();
    TCB0_Init();
    EVSYS_Init();
    ZCD0_Init();
    PORT_Init();

    while(1)
    {
        ;
    }
}

ISR(TCB0_INT_vect)
{
    /**
    * The frequency is stored in the CCMP register
    * An equation can be used to transform the value in Hz
    */
    signal_frequency = TCB0.CCMP;
}
