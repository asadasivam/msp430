/*
 * uart_echo.c
 *
 *  Created on: Mar 20, 2021
 *      Author: sadasivam
 */
#include <msp430.h>
#include "uart_echo.h"
unsigned char getBufferFullState = 0;
unsigned char receiveByte = 0;
void uart_config()
{

    P2SEL1 |= BIT0 | BIT1;                    // USCI_A0 UART operation
    P2SEL0 &= ~(BIT0 | BIT1);

    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));
      receiveByte = UCA0RXBUF;
      if(receiveByte == 0)
      {
          //receiving NULL character
          getBufferFullState = true;
      }
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}

