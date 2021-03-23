#include <msp430.h>
#include "uart_echo.h"
#include "volatile_mem.h"

//globals from uart_echo.h
//unsigned char getBufferFullState = 0;

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

  PJOUT = 0;
  PJDIR = 0xFFFF;

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Startup clock system with max DCO setting ~8MHz
  CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
  CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
  CSCTL0_H = 0;                             // Lock CS registers

  uart_config();

  while(!getBufferFullState)
  {
    __bis_SR_register(LPM4_bits | GIE);     // Enter LPM4 w/interrupt
    __no_operation();                       // For debugger
    P1OUT ^= BIT0;                          // P1.0 = toggle
    volatile_memCheck();
  }
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    //reading the button interrupt count within a period of time
    P1IES ^= BIT1;                            // Toggle interrupt edge
    P1IFG &= ~BIT1;                           // Clear P1.1 IFG
    __bic_SR_register_on_exit(LPM4_bits);     // Exit LPM4
}



