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
#define NORMALLED _RD0
#define OLLED _RC13
#define FLTLED _RE8

#define SAMPLE 4
#define SAMPLE1 4

#define BUZZER _RC14
uint32_t inputvoltage=0;
uint32_t outputvoltage=0;
static uint32_t out1[20],out2[20],in1[20];
uint32_t outputcurrent=0;
static uint16_t timePeriod[10];
uint16_t i=0,j=0;

volatile bool PWM_Halfcycle_chk=false;
volatile bool Prev_Halfcycle_chk=false;
volatile bool Int_chk=false;
unsigned char PWM_BstBk_chk=0;
volatile bool bypass_chk=false;
uint16_t seccount=0,mseccount=0;
volatile bool dutycycle_chk=false;
volatile bool sec_chk=false;
volatile bool sw=false;

//Functions and Variables with Global Scope:

void __attribute__((__interrupt__)) _ADCInterrupt(void);
void __attribute__((__interrupt__)) _IC2Interrupt(void);
void __attribute__((__interrupt__)) _T3Interrupt(void);
void __attribute__((__interrupt__)) _INT2Interrupt(void);
void __attribute__((__interrupt__)) _T1Interrupt(void);
void __attribute__((__interrupt__)) _CNInterrupt(void);
void __attribute__((__interrupt__)) _FLTAInterrupt(void);

extern void PID_Update(void);

void __attribute__((interrupt, no_auto_psv)) _FLTAInterrupt(void)
{
    bypass_chk=true;
       NORMALLED=1;
        BKLED=1;
        BSTLED=1;

        OVDCONbits.POVD1L = 0;
        OVDCONbits.POVD2L = 0;
        sw=false;
        _FLTAIF=0;
        //PDC1 =0;
       // PDC2 = 0;
     //   propational=0;
       //         integral_error=0;
         //       pid =0;

}


//_ADCInterrupt() is the A/D interrupt service routine (ISR).
//The routine must have global scope in order to be an ISR.
//The ISR name is chosen from the device linker script.
void __attribute__((interrupt, no_auto_psv)) _ADCInterrupt(void)
{
	//ADResult1 = ADCBUF0;
    inputvoltage = inputvoltage - (inputvoltage>>SAMPLE)+(uint32_t)ADCBUF1;
    outputvoltage =outputvoltage - (outputvoltage>>SAMPLE1)+ (uint32_t)ADCBUF2;
    outputcurrent = outputcurrent - (outputcurrent>>SAMPLE)+(uint32_t)ADCBUF3;
    //dcdc_avg1= dcdc_avg1 - (dcdc_avg1>>DCDCVOLT_AVG) + DCDC_Out1;
    //outputvoltage >>=2;
    in1[j]=inputvoltage>>SAMPLE;
    out1[j]=outputvoltage>>SAMPLE1;
    out2[j]=outputcurrent>>SAMPLE;
    j++;
    if(j>19)
        j=0;
    //outputcurrent = ADCBUF3;
	ADResult2 = ADCBUF0;
  //      ADResult3 = ADCBUF2;
       // ADResult4 = ADCBUF3;

        //Clear the A/D Interrupt flag bit or else the CPU will
        //keep vectoring back to the ISR
        IFS0bits.ADIF = 0;


}
void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{
    
    if(_RB3==0)
    {
        if(_RB3==0)
        {
           // NORMALLED=1;
            sw=true;
            BUZZER=0;
            PTCONbits.PTEN = 1;
        }
    }
    else
    {
        if(_RB3==1)
        {
         //   NORMALLED=0;
            sw=false;
            BUZZER=1;

            PTCONbits.PTEN = 1;
        }
    }

    _CNIF=0;
}
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
   // FLTLED=1;
   // NORMALLED=1;

    // 100 us timer

    //OLLED=~OLLED;
    mseccount++;
    if(mseccount>15)
    {
        mseccount=0;
        dutycycle_chk=true;
        seccount++;
        //if(seccount>200) // 1 sec
            if(seccount>800) // .5 sec
        {
            seccount=0;
            sec_chk=true;
        }
    }
    IFS0bits.T1IF=0;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
   // FLTLED=1;
   // NORMALLED=1;
    IFS0bits.T3IF=0;
}
void __attribute__((interrupt, no_auto_psv)) _IC2Interrupt(void)
{
    
    //BSTLED = ~BSTLED;
    //t1=IC2BUF;
    //_IC2IF=0;
    unsigned int t1,t2;
//OLLED=~OLLED;
t1=IC2BUF;
t2=IC2BUF;
//BSTLED = ~BSTLED;
//NORMALLED = PORTDbits.RD1^NORMALLED;
IFS0bits.IC2IF=0;
if(t2>t1)
timePeriod[i] = t2-t1;
else
timePeriod[i] = (PR3 - t1) + t2;
//if((timePeriod[i] >=0xFA00)&&(timePeriod[i]<=0xFB00))
/*if(Int_chk)
{
    Int_chk=false;
 // NORMALLED=~NORMALLED;
    if(sw==true)
    {
    if(bypass_chk==false)
    {
   if(PWM_BstBk_chk)
   {
    if(PWM_Halfcycle_chk & !Prev_Halfcycle_chk)
  {
        Prev_Halfcycle_chk=true;
      //if(!PTCONbits.PTEN) PTCONbits.PTEN = 1;     /* Turn ON PWM module */
  //    OVDCONbits.POVD1L = 0;    //override
    //OVDCONbits.POVD2L = 1;    //override
      //OVDCONbits.POVD1L =1;
      //OVDCONbits.POVD2L = 1;
    //  _PEN1L = 1;
    //  _PEN2L = 1;
 /* }
  else if(!PWM_Halfcycle_chk & Prev_Halfcycle_chk)
  {
        Prev_Halfcycle_chk=false;
      OVDCONbits.POVD1L = 1;
      OVDCONbits.POVD2L = 0;
      //OVDCONbits.POVD1L =0;
     // OVDCONbits.POVD2L = 0;
  }
   }
  else
   {
    if(PWM_Halfcycle_chk & !Prev_Halfcycle_chk)
  {
Prev_Halfcycle_chk=true;
      //if(!PTCONbits.PTEN) PTCONbits.PTEN = 1;     /* Turn ON PWM module */
   /*   OVDCONbits.POVD1L = 1;    //override
      OVDCONbits.POVD2L = 0;    //override
      //OVDCONbits.POVD1L =1;
      //OVDCONbits.POVD2L = 1;
    //  _PEN1L = 1;
    //  _PEN2L = 1;
  }
  else if(!PWM_Halfcycle_chk & Prev_Halfcycle_chk)
  {
        Prev_Halfcycle_chk=false;

      OVDCONbits.POVD1L = 0;
      OVDCONbits.POVD2L = 1;
      //OVDCONbits.POVD1L =0;
     // OVDCONbits.POVD2L = 0;
  }
   }
    }
}
}*/
//i++;
//if(i>10)
  //  i=0;
}
void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void)
{
    Int_chk=true;
    if(INTCON2bits.INT2EP)
    {
        INTCON2bits.INT2EP=0;
      //  NORMALLED=0;
        PWM_Halfcycle_chk=0;
    }
    else
    {
        INTCON2bits.INT2EP=1;
        //NORMALLED=1;
        PWM_Halfcycle_chk=1;
    }
      if(Int_chk)
        {
        Int_chk=false;
        // NORMALLED=~NORMALLED;
        if(sw==true)
            {
            if(bypass_chk==false)
                {
                if(PWM_BstBk_chk)
                    {
                    if(PWM_Halfcycle_chk & !Prev_Halfcycle_chk)
                        {
                        Prev_Halfcycle_chk=true;
                        //if(!PTCONbits.PTEN) PTCONbits.PTEN = 1;     /* Turn ON PWM module */
                        OVDCONbits.POVD1L = 0;    //override
                        OVDCONbits.POVD2L = 1;    //override
                        //OVDCONbits.POVD1L =1;
                        //OVDCONbits.POVD2L = 1;
                        //  _PEN1L = 1;
                        //  _PEN2L = 1;
                        }
                    else if(!PWM_Halfcycle_chk & Prev_Halfcycle_chk)
                        {
                        Prev_Halfcycle_chk=false;
                        OVDCONbits.POVD1L = 1;
                        OVDCONbits.POVD2L = 0;
                        //OVDCONbits.POVD1L =0;
                        // OVDCONbits.POVD2L = 0;
                        }
                    }
                 else
                    {
                    if(PWM_Halfcycle_chk & !Prev_Halfcycle_chk)
                        {
                        Prev_Halfcycle_chk=true;
                        //if(!PTCONbits.PTEN) PTCONbits.PTEN = 1;     /* Turn ON PWM module */
                        OVDCONbits.POVD1L = 1;    //override
                        OVDCONbits.POVD2L = 0;    //override
                        //OVDCONbits.POVD1L =1;
                        //OVDCONbits.POVD2L = 1;
                        //  _PEN1L = 1;
                        //  _PEN2L = 1;
                        }
                    else if(!PWM_Halfcycle_chk & Prev_Halfcycle_chk)
                        {
                        Prev_Halfcycle_chk=false;
                        OVDCONbits.POVD1L = 0;
                        OVDCONbits.POVD2L = 1;
                        //OVDCONbits.POVD1L =0;
                        // OVDCONbits.POVD2L = 0;
                        }
                    }
                }
            }
    }
IFS1bits.INT2IF=0;
}