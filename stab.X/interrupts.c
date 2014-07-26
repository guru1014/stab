/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p30Fxxxx.h>      /* Device header file                              */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

/******************************************************************************/
/* Interrupt Vector Options                                                   */
/******************************************************************************/
/*                                                                            */
/* Refer to the C30 (MPLAB C Compiler for PIC24F MCUs and dsPIC33F DSCs) User */
/* Guide for an up to date list of the available interrupt options.           */
/* Alternately these names can be pulled from the device linker scripts.      */
/*                                                                            */
/* Primary Interrupt Vector Names:                                            */
/*                                                                            */
/* _INT0Interrupt  _INT2Interrupt                                             */
/* _IC1Interrupt   _U2RXInterrupt                                             */
/* _OC1Interrupt   _U2TXInterrupt                                             */
/* _T1Interrupt    _SPI2Interrupt                                             */
/* _IC2Interrupt   _C1Interrupt                                               */
/* _OC2Interrupt   _IC3Interrupt                                              */
/* _T2Interrupt    _IC4Interrupt                                              */
/* _T3Interrupt    _IC5Interrupt                                              */
/* _SPI1Interrupt  _IC6Interrupt                                              */
/* _U1RXInterrupt  _OC5Interrupt                                              */
/* _U1TXInterrupt  _OC6Interrupt                                              */
/* _ADCInterrupt   _OC7Interrupt                                              */
/* _NVMInterrupt   _OC8Interrupt                                              */
/* _SI2CInterrupt  _INT3Interrupt                                             */
/* _MI2CInterrupt  _INT4Interrupt                                             */
/* _CNInterrupt    _C2Interrupt                                               */
/* _INT1Interrupt  _PWMInterrupt                                              */
/* _IC7Interrupt   _QEIInterrupt                                              */
/* _IC8Interrupt   _DCIInterrupt                                              */
/* _OC3Interrupt   _LVDInterrupt                                              */
/* _OC4Interrupt   _FLTAInterrupt                                             */
/* _T4Interrupt    _FLTBInterrupt                                             */
/* _T5Interrupt                                                               */
/*                                                                            */
/* For alternate interrupt vector naming, simply add 'Alt' between the prim.  */
/* interrupt vector name '_' and the first character of the primary interrupt */
/* vector name.                                                               */
/*                                                                            */
/* For example, the vector name _ADC2Interrupt becomes _AltADC2Interrupt in   */
/* the alternate vector table.                                                */
/*                                                                            */
/* Example Syntax:                                                            */
/*                                                                            */
/* void __attribute__((interrupt,auto_psv)) <Vector Name>(void)               */
/* {                                                                          */
/*     <Clear Interrupt Flag>                                                 */
/* }                                                                          */
/*                                                                            */
/* For more comprehensive interrupt examples refer to the C30 (MPLAB C        */
/* Compiler for PIC24 MCUs and dsPIC DSCs) User Guide in the                  */
/* <compiler installation directory>/doc directory for the latest compiler    */
/* release.                                                                   */
/*                                                                            */
/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* TODO Add interrupt routine code here. */
unsigned int ADResult1 = 0;
unsigned int ADResult2 = 0;
unsigned int ADResult3 = 0;
unsigned int ADResult4 = 0;

#define BKLED _RB4
#define BSTLED  _RB5
#define BYPASSLED _RD0
#define FLTLED _RE8

uint16_t inputvoltage=0;
uint16_t outputvoltage=0;
uint16_t outputcurrent=0;
static uint16_t timePeriod[10];
uint16_t i=0;

//Functions and Variables with Global Scope:

void __attribute__((__interrupt__)) _ADCInterrupt(void);
void __attribute__((__interrupt__)) _IC2Interrupt(void);
void __attribute__((__interrupt__)) _T3Interrupt(void);



//_ADCInterrupt() is the A/D interrupt service routine (ISR).
//The routine must have global scope in order to be an ISR.
//The ISR name is chosen from the device linker script.
void __attribute__((interrupt, no_auto_psv)) _ADCInterrupt(void)
{
	//ADResult1 = ADCBUF0;
    inputvoltage = ADCBUF0;
    outputvoltage = ADCBUF1;
    outputcurrent = ADCBUF2;
//	ADResult2 = ADCBUF1;
  //      ADResult3 = ADCBUF2;
        ADResult4 = ADCBUF3;

        //Clear the A/D Interrupt flag bit or else the CPU will
        //keep vectoring back to the ISR
        IFS0bits.ADIF = 0;


}
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    FLTLED=1;
    BYPASSLED=1;
    IFS0bits.T3IF=0;
}
void __attribute__((interrupt, no_auto_psv)) _IC2Interrupt(void)
{
    
    //BSTLED = ~BSTLED;
    //t1=IC2BUF;
    //_IC2IF=0;
    unsigned int t1,t2;
t1=IC2BUF;
t2=IC2BUF;
BSTLED = ~BSTLED;
IFS0bits.IC2IF=0;
if(t2>t1)
timePeriod[i] = t2-t1;
else
timePeriod[i] = (PR3 - t1) + t2;
i++;
if(i>10)
    i=0;
}