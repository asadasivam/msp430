; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2012, Texas Instruments Incorporated
;  All rights reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
;
;  *  Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;
;  *  Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
;
;  *  Neither the name of Texas Instruments Incorporated nor the names of
;     its contributors may be used to endorse or promote products derived
;     from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
;  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
;  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
;  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
;  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
;  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
;  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
;  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
;  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
;  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
; ******************************************************************************
;
;                        MSP430 CODE EXAMPLE DISCLAIMER
;
;  MSP430 code examples are self-contained low-level programs that typically
;  demonstrate a single peripheral function or device feature in a highly
;  concise manner. For this the code may rely on the device's power-on default
;  register values and settings such as the clock configuration and care must
;  be taken when combining code from several examples to avoid potential side
;  effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
;  for an API functional library-approach to peripheral configuration.
;
; --/COPYRIGHT--
;******************************************************************************
;  MSP430FR59x Demo - Timer0_A3, Toggle P1.0, Overflow ISR, 32kHz ACLK
;
;  Description: Toggle P1.0 using software and the Timer0_A overflow ISR.
;  In this example an ISR triggers when TA overflows. Inside the ISR P1.0
;  is toggled. Toggle rate is exactly 0.5Hz. Proper use of the TAIV interrupt
;  vector generator is demonstrated.
;
;  ACLK = TACLK = 32768Hz, MCLK = SMCLK = DCO / 2 = 8MHz / 2 = 4MHz
;
;
;                MSP430FR5969
;             -----------------
;         /|\|              XIN|-
;          | |                 |  32KHz Crystal
;          --|RST          XOUT|-
;            |                 |
;            |             P1.0|--> LED
;
;   E. Chen
;   Texas Instruments Inc.
;   October 2013
;   Built with Code Composer Studio V5.5
;******************************************************************************
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global _main
            .global __STACK_END
            .sect   .stack                  ; Make stack linker segment ?known?

            .text                           ; Assemble to Flash memory
            .retain                         ; Ensure current section gets linked
            .retainrefs

_main
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
SetupGPIO   bis.b   #BIT0,&P1DIR            ; Set P1.0 to output direction
            bis.b   #BIT0,&P1OUT
            bis.w   #BIT4+BIT5,&PJSEL0

UnlockGPIO  bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

SetupCS     mov.b   #CSKEY_H,&CSCTL0_H      ; Unlock CS registers
            mov.w   #DCOFSEL_6,&CSCTL1      ; Set DCO to 8MHz
            mov.w   #SELA__LFXTCLK+SELS__DCOCLK+SELM__DCOCLK,&CSCTL2 ; Set ACLK = XT1; MCLK = DCO
            mov.w   #DIVA__1+DIVS__2+DIVM__2,&CSCTL3  ; Set all dividers
            bic.w   #LFXTOFF,&CSCTL4

OSCFlag     bic.w   #LFXTOFFG,&CSCTL5       ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jnz     OSCFlag
            clr.b   &CSCTL0_H               ; Lock CS registers

            mov.w   #TASSEL__ACLK+MC__CONTINUOUS+TACLR+TAIE,&TA0CTL ; ACLK, contmode, clear TAR
                                            ; enable interrupt
            nop                             ; 
            bis.w   #LPM3+GIE,SR            ; Enter LPM3 w/ interrupt
            nop                             ; for debug

;-------------------------------------------------------------------------------
TIMER0_A1_ISR;    Timer0_A3 CC1-2 Interrupt Service Routine
;-------------------------------------------------------------------------------
            add.w   &TA0IV,PC               ; add offset to PC
            reti                            ; Vector  0:  No interrupt
            reti                            ; Vector  2:  CCR1 not used
            reti                            ; Vector  4:  CCR2 not used
            reti                            ; Vector  6:  reserved
            reti                            ; Vector  8:  reserved
            reti                            ; Vector 10:  reserved
            reti                            ; Vector 12:  reserved
            jmp     MEM0                    ; Vector 14:  overflow
MEM0        xor.b   #BIT0,&P1OUT            ; Toggle LED
            reti
;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   TIMER0_A1_VECTOR        ; Timer0_A3 CC1-2 Interrupt Vector
            .short  TIMER0_A1_ISR           ;
            .end
