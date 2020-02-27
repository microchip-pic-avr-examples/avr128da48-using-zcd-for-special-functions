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

void ZCD0_Init(void);
void ZCD1_Init(void);
void PORT_Init(void);
void EVSYS_Init(void);

/* Initialization of the IO Ports */
void PORT_Init(void)
{
    /* Disable digital input buffer and pull-up resistor on PE3*/
    PORTE.PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;

    /* Disable digital input buffer and pull-up resistor on PD1*/
    PORTD.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
    
    /* SET PB2 as an output*/
    PORTB.OUTSET |= PIN2_bm;
    PORTB.DIRSET |= PIN2_bm;
    
    /* SET PC2 as an output*/
    PORTC.OUTSET |= PIN2_bm;
    PORTC.DIRSET |= PIN2_bm;
}

/* Initialization of the Zero Cross Detector */
void ZCD0_Init(void)
{    
    ZCD0.CTRLA = ZCD_ENABLE_bm;        /* Enable the ZCD1 */
}

void ZCD1_Init(void)
{
    ZCD1.CTRLA = ZCD_ENABLE_bm    /* Enable the ZCD1*/
               | ZCD_INVERT_bm;    /* Invert the ZCD1 OUTPUT */
}

/* Enable event generation from ZCD to Event system OUTPUT I/O pins */
void EVSYS_Init(void)
{
    EVSYS.CHANNEL0 = EVSYS_CHANNEL0_ZCD0_gc;     /* Zero-cross detector 0 out linked to Event Channel 0 */
    EVSYS.USEREVSYSEVOUTB = EVSYS_CHANNEL00_bm;  /* Event system output B (PB2) uses Event Channel 0 */
    
    EVSYS.CHANNEL1 = EVSYS_CHANNEL1_ZCD1_gc;     /* Zero-cross detector 1 out linked to Event Channel 1 */
    EVSYS.USEREVSYSEVOUTC = EVSYS_CHANNEL01_bm;  /* Event system output C (PC2) uses Event Channel 1 */
}


int main(void)
{
    EVSYS_Init();
    ZCD0_Init();
    ZCD1_Init();
    PORT_Init();
    
    /* Replace with your application code */
    while(1)
    {
        ;
    }
}
