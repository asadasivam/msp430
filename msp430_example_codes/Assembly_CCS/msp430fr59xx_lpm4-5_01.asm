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
;   MSP430FR59xx Demo - Entering and waking up from LPM4.5 via P1.1 interrupt
;
;   Description: Download and run the program. LED1 (or P4.6) will remain ON if
;                LPM4.5 is correctly entered. Use a button S3 (or P1.1) on the
;                EXP board to wake the device up from LPM4.5. This will enable
;                the LFXT oscillator and blink the LED2 (on P1.0).
;
;                This demo was tested on MSP-EXP430FR5969 LaunchPad.
;
;           MSP430FR5969
;         ---------------
;     /|\|            XIN|-
;      | |               | 32KHz Crystal
;      --|RST        XOUT|-
;        |               |
;        |           P1.0|---> LED2 (MSP-EXP430FR5969)
;        |           P4.6|---> LED1 (MSP-EXP430FR5969)
;        |               |
;        |           P1.1|<--- S3 push-button (MSP-EXP430FR5969)
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
SetupGPIO   clr.b   &P1OUT                  ; Pull-up resistor on P1.1
            mov.b   #0xFF,&P1DIR            ; Set all but P1.1 to output direction

            clr.b   &P2OUT
            mov.b   #0xFF,&P2DIR

            clr.b   &P3OUT
            mov.b   #0xFF,&P3DIR

            clr.b   &P4OUT
            mov.b   #0xFF,&P4DIR

            clr.w   &PJOUT                  ; Set PJ.4 / LFXTIN to high
            mov.w   #0xFFFF,&PJDIR

            cmp.w   #SYSRSTIV_LPM5WU,SYSRSTIV ; Determine whether we are coming out of an LPMx.5 or a regular RESET.
            jne     RegularRST
            mov.w   #BIT4,&PJSEL0           ; For XT1

            mov.b   #CSKEY_H,&CSCTL0_H      ; Unlock CS registers
            bis.w   #DCOFSEL_0,&CSCTL1      ; Set DCO to 1MHz
            mov.w   #SELA__LFXTCLK+SELS__DCOCLK+SELM__DCOCLK,&CSCTL2
            mov.w   #DIVA__1+DIVS__1+DIVM__1,&CSCTL3  ; Set all dividers
            bic.w   #LFXTOFF,&CSCTL4

            bis.b   #BIT0,&P1DIR            ; Configure LED pin for output

            bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

OSCFlag     bic.w   #LFXTOFFG,&CSCTL5       ; Clear XT1 fault flag
            bic.w   #OFIFG,&SFRIFG1
            bit.w   #OFIFG,&SFRIFG1         ; Test oscillator fault flag
            jnz     OSCFlag
            jmp     Mainloop

RegularRST  bis.b   #BIT1,&P1OUT            ; Pull-up resistor on P1.1
            bis.b   #BIT1,&P1REN            ; Select pull-up mode for P1.1
            mov.b   #0xFF^BIT1,&P1DIR       ; Set all but P1.1 to output direction
            bis.b   #BIT1,&P1IES            ; P1.1 Hi/Lo edge
            clr.b   &P1IFG                  ; Clear all P1 interrupt flags
            bis.b   #BIT1,&P1IE             ; P1.1 interrupt enabled

            bis.b   #BIT6,&P4OUT            ; Turn on P4.6 (LED1) on EXP board
                                            ; to indicate we are about to enter
                                            ; LPM4.5

            bic.w   #LOCKLPM5,&PM5CTL0      ; Disable the GPIO power-on default
                                            ; high-impedance mode to activate
                                            ; previously configured port settings

            mov.b   #PMMPW_H,&PMMCTL0_H     ; Open PMM Registers for write
            bis.b   #PMMREGOFF,&PMMCTL0_L   ; and set PMMREGOFF
            clr.b   &PMMCTL0_H              ; Lock PMM Registers

            bis.w   #LPM4,SR                ; Enter LPM4 Note that this operation does
                                            ; not return. The LPM4.5 will exit through a RESET
                                            ; event, resulting in a re-start of the code.

Mainloop    xor.b   #BIT0,&P1OUT            ; P1.0 = toggle
Wait        mov.w   #50000,R15              ; Delay to R15
L1          dec.w   R15                     ; Decrement R15
            jnz     L1                      ; Delay over?
            jmp     Mainloop                ; Again

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .end

