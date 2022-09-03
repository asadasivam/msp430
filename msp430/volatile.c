//******************************************************************************
//  MSP430FR59xx Demo - Software Port Interrupt Service on P1.1 from LPM4
//
//  Description: A Hi/Lo transition on P1.1 will trigger P1ISR the first time.
//  On hitting the P1ISR, device exits LPM4 mode and executes section of code in
//  main() which includes toggling an LED and the P1.1 IES bit which switches
//  between Lo/Hi and Hi/Lo trigger transitions to alternatively enter the P1ISR.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//
//               MSP430FR5969
//            -----------------
//        /|\|                 |
//         | |                 |
//         --|RST              |
//     /|\   |                 |
//      --o--|P1.1         P1.0|-->LED
//     \|/
//
//   T. Witt/ P. Thanigai
//   Texas Instruments Inc.
//   August 2012
//   Built with IAR Embedded Workbench V5.30 & Code Composer Studio V5.5
//******************************************************************************

#include <msp430.h>
//port reading
#define uint16_t unsigned int
#define baseAddress __MSP430_BASEADDRESS_PORT1_R__
#define PORT1_IN    (*((volatile uint16_t *)((uint16_t)baseAddress + (0x0000))))
//try to comment volatile from macro-PORT1_IN and optimize whole program and 
//see whether the IN buffer read. It will not. So, its one of the use of volatile.

unsigned char arr[20] = {0};
unsigned char i = 0;
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

  // Configure GPIO
  P1OUT = BIT1;                             // Pull-up resistor on P1.1
  P1REN = BIT1;                             // Select pull-up mode for P1.1
  P1DIR = 0xFF ^ BIT1;                      // Set all but P1.1 to output direction
  P1IES = BIT1;                             // P1.1 Hi/Lo edge
  P1IFG = 0;                                // Clear all P1 interrupt flags
  P1IE = BIT1;                              // P1.1 interrupt enabled

  P2OUT = 0;
  P2DIR = 0xFF;

  P3OUT = 0;
  P3DIR = 0xFF;

  P4OUT = 0;
  P4DIR = 0xFF;

  PJOUT = 0;
  PJDIR = 0xFFFF;

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  while(1)
  {
    __bis_SR_register(LPM4_bits | GIE);     // Enter LPM4 w/interrupt
    __no_operation();                       // For debugger
    P1OUT ^= BIT0;                          // P1.0 = toggle
    for(i = 0; i<20; i++)
    {
        arr[i] = PORT1_IN; //reading input buffer 100 times
    }
  }
}

// Port 1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
  P1IES ^= BIT1;                            // Toggle interrupt edge
  P1IFG &= ~BIT1;                           // Clear P1.1 IFG
  __bic_SR_register_on_exit(LPM4_bits);     // Exit LPM4
}
