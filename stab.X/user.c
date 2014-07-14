/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p30Fxxxx.h>        /* Device header file                            */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "user.h"            /* variables/params used by user.c               */

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */
void ADC_Init(void);
void PWM_Init(void);
/* TODO Initialize User Ports/Peripherals/Project here */

void InitApp(void)
{
    TRISB = 0x07;
    PORTB =0x10;
    TRISE = 0x00;
    
    /* Setup analog functionality and port direction */
    ADC_Init();
    PWM_Init();
    /* Initialize peripherals */
}

//Functions:
//ADC_Init() is used to configure A/D to convert 16 samples of 1 input
//channel per interrupt. The A/D is set up for a sampling rate of 1MSPS
//Timer3 is used to provide sampling time delay.
//The input pin being acquired and converted is AN7.
void ADC_Init(void)
{
        //ADCON1 Register
        //Set up A/D for Automatic Sampling
        //Use internal counter (SAMC) to provide sampling time
        //Set up A/D conversrion results to be read in 1.15 fractional
        //number format.
        //Set up Sequential sampling for multiple S/H amplifiers
        //All other bits to their default state
        ADCON1bits.FORM = 0;   //
        ADCON1bits.SSRC = 7;
        ADCON1bits.ASAM = 1;
        ADCON1bits.SIMSAM = 1;

        //ADCON2 Register
        //Set up A/D for interrupting after 2 samples get filled in the buffer
        //Set up to sample on 2 S/H amplifiers - CH0, CH1, CH2 and CH3
        //All other bits to their default state
        ADCON2bits.SMPI = 1;
        ADCON2bits.CHPS = 3;
	//ADCON2bits.VCFG = 3; //Ideally use external references

        //ADCON3 Register
        //We would like to set up a sampling rate of 1 MSPS
        //Total Conversion Time= 1/Sampling Rate = 125 microseconds
        //At 29.4 MIPS, Tcy = 33.9 ns = Instruction Cycle Time
        //The A/D converter will take 12*Tad periods to convert each sample
        //So for ~1 MSPS we need to have Tad close to 83.3ns
        //Using equaion in the Family Reference Manual we have
        //ADCS = 2*Tad/Tcy - 1
        ADCON3bits.SAMC = 0;
        ADCON3bits.ADCS = 4;

        //ADCHS Register
        //Set up A/D Channel Select Register to convert AN3 on Mux A input
        //of CH0 and CH1 S/H amplifiers
        ADCHS = 0x0000;

        //ADCSSL Register
        //Channel Scanning is disabled. All bits left to their default state
        ADCSSL = 0x0000;

        //ADPCFG Register
        //Set up channels AN7 as analog input and configure rest as digital
        //Recall that we configured all A/D pins as digital when code execution
        //entered main() out of reset
        ADPCFG = 0xFFF8;
       // ADPCFGbits.PCFG3 = 0;

        //Clear the A/D interrupt flag bit
        IFS0bits.ADIF = 0;

        //Set the A/D interrupt enable bit
        IEC0bits.ADIE = 1;

        //Turn on the A/D converter
        //This is typically done after configuring other registers
        ADCON1bits.ADON = 1;

}
//Functions:
//PWM_Init() is used to configure PWM
void PWM_Init(void)
{
     PTPER = 682;            /* PWM period of approx. 500 nsec
                               PWM Period = PTPER*1.05nsec = 537.6 nsec */

    PDC1 = 682;             /* PWM1 pulse width of 250 nsec
                               Duty Cycle = PDC1*1.05nsec = 268.8 nsec */

    PDC2 = 682;             /* PWM2 pulse width of 250 nsec
                               Duty Cycle = PDC2*1.05nsec = 268.8 nsec */

PDC3 = 682;             /* PWM2 pulse width of 250 nsec
                               Duty Cycle = PDC2*1.05nsec = 268.8 nsec */

    /* Note that a pulse appears only on every other PWM cycle. So in push-pull
       mode, the effective duty cycle is 25% */

    DTCON1bits.DTAPS = 0;
    DTCON1bits.DTA  =2;/* 33.6 nsec dead time
                               Dead-time = DTR1*1.05nsec = 33.6 nsec */

    DTCON1bits.DTBPS = 0;

    DTCON1bits.DTB  =2;/* 33.6 nsec dead time
                               Dead-time = DTR2*1.05nsec = 33.6 nsec */
//    PTCONbits.PTCKPS = 3;
    PTCONbits.PTMOD = 0;
    _PEN1L=1;
    _PEN1H=1;
    _PEN2L=1;
    _PEN2H=1;
    _PEN3L=1;
    _PEN3H=1;



_POUT1L=0;
_POUT1H=1;
_POUT2L=0;
_POUT2H=1;
_POUT3L=0;
_POUT3H=1;

   // ALTDTR1 = 32;           /* 33.6 nsec dead time
     //                          Alt Dead-time = ALTDTR1*1.05nsec = 33.6 nsec */

   // ALTDTR2 = 32;           /* 33.6 nsec dead time
  //                             Alt Dead-time = ALTDTR2*1.05nsec = 33.6 nsec */

   // PHASE1 = 0;             /* No phase shift for PWM1 */
  //  PHASE2 = 128;           /* Phase shift of 128 nsec for PWM2
     //                          Phase Shift = PHASE2*1.05nsec = 134.4 nsec */

  //  IOCON1bits.PENH = 1;    /* PWM1H output controlled by PWM */
  //  IOCON1bits.PENL = 1;    /* PWM1L output controlled by PWM */
  //  IOCON1bits.PMOD = 2;    /* Select Push-Pull PWM mode */

  //  IOCON2bits.PENH = 1;    /* PWM2H output controlled by PWM */
  //  IOCON2bits.PENL = 1;    /* PWM2L output controlled by PWM */
  //  IOCON2bits.PMOD = 2;    /* Select Push-Pull PWM mode */

    PTCONbits.PTEN = 1;     /* Turn ON PWM module */
}

