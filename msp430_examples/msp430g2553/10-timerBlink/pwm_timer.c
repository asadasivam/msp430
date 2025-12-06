#include <msp430.h>

#define GREEN	BIT6			// Green LED -> P1.6
/*
sDRV8816 : DMOS dual 1/2 H-bridge motor drives
stepper motor - NEMA 17 5 KG torque 1.8
DRV8871 : 3.6 a Brushed DC motor driver with internal current sense (PWM control)
*/

int main(void)    
{
      WDTCTL = WDTPW + WDTHOLD;		// Stop WDT
      P1DIR |= GREEN;			// Green LED -> Output
      P1SEL |= GREEN;	// Green LED -> Select Timer Output

      //app001:	timer config for 0.25s, 0.5s, 0.75s, 1s delay led output 
   
      //app002: led driver to drive 6 leds and verify the current drawn in leds output - simulation
   
      //app003: adc config for led voltage monitoring 
   
      P1DIR |= BIT0;			//led output
      P1OUT |= BIT0;			//high output

      CCR0 = 500;			// Set Timer0 PWM Period
      CCTL1 = OUTMOD_7;			// Set TA0.1 Waveform  - Clear on Compare, Set on Overflow
      CCR1 = 230;			// Set TA0.1 PWM duty cycl
      TACTL = TASSEL_2 + MC_1;		// Timer Clock -> SMCLK, Mode -> Up Count

      __bis_SR_register(LPM0_bits);     // Enter LPM0
      __no_operation();                 // For debugger   

	while(1)
	{
		unsigned int i;
		for(i = 0; i < 500; i++)
		{
			CCR1 = i;						// Increase Duty from min to max
			__delay_cycles(5000);
		}
		for(i = 500; i > 0; i--)
		{
			CCR1 = i;						// Decrease Duty from max to min
			__delay_cycles(5000);
		}
	}
	return 0;
}