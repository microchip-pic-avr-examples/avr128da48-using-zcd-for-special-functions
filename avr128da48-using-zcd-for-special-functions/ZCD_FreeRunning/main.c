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
void PORT_Init(void);

/* Initialization of the Zero Cross Detector */
void ZCD0_Init(void)
{ 
	ZCD0.CTRLA = ZCD_ENABLE_bm	/* Enable the ZCD;*/
	           | ZCD_OUTEN_bm;	/*Enable the output of the ZCD*/
}

void PORT_Init(void)
{
    /* Disable digital input buffer and pull-up resistor on PD1*/
    PORTD.PIN1CTRL |= PORT_ISC_INPUT_DISABLE_gc;
    
    /* SET PA7 as an output*/
    PORTA.DIRSET |= PIN7_bm;
    PORTA.OUTSET |= PIN7_bm;
}

int main(void)
{
	ZCD0_Init();
    PORT_Init();
	
    /* Replace with your application code */
    while(1) 
    {
        ;
    }
}
