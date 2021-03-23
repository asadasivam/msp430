/*
 * project      : uart_echo
 * description  : Echo back the character using serial terminal
 *                if char was NULL, it will read last val from buffer
 *
 */

#include "msp430.h"
#include "echo.h"
#include "buffer.h"
#define NULL 0

unsigned char receive_flag = 0;//extern unsigned char var;
unsigned int get_char = 0;
unsigned int buffer_status = 0;
int current_val = 0;
//pointer to function return int val
int (*ptrfun)(int) = getBufferVal;
//int (*ptrfun)() = getBufferVal;
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog

  // Configure GPIO
  P2SEL1 |= BIT0 | BIT1;                    // USCI_A0 UART operation
  P2SEL0 &= ~(BIT0 | BIT1);

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

  while(1)
  {
  __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, interrupts enabled
  //__no_operation();                         // For debugger
  if(receive_flag == 1)
  {
   //get the buffer value by pointer
      current_val = (*ptrfun)(bufferTop);
  }
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));
      get_char = UCA0RXBUF;
      pushBuffer(get_char); //filling large uart buffer
      if(get_char == NULL)
      {
          receive_flag = 1;
      }
      //disable interrupt
      __bic_SR_register_on_exit(LPM3_bits);
      //__no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}
