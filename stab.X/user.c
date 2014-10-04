/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p30Fxxxx.h>        /* Device header file                            */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include <dsp.h>
#include <libq.h>
#include "user.h"            /* variables/params used by user.c               */

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */
void ADC_Init(void);
void PWM_Init(void);
void InitTMR3(void);
void InitTMR1(void);
void Self_Test(void);
void InitPID(void);
//void Read_ADC(void);
/* TODO Initialize User Ports/Peripherals/Project here */
const uint16_t *ADC16ptr;
uint16_t count;
int k=0;
bool Bst_flag=false;
//
float error;
float current_value=0,last=0;
float setpoint=475;
float integral_error=0;
float propational=0;
float integral=0;
float derivative = 0;
float kp=0.005;
float ki=0.005;
float kd=0;
float pid=0;
unsigned int normalcount=0;
//float e[20],i=0;
//
void PID_Update(void);

/* Declare a PID Data Structure named, stabPID */
//tPID stabPID;

/* The fooPID data structure contains a pointer to derived coefficients in X-space and */
/* pointer to controler state (history) samples in Y-space. So declare variables for the */
/* derived coefficients and the controller history samples */
//fractional abcCoefficient[3] __attribute__ ((section (".xbss, bss, xmemory")));
//fractional controlHistory[3] __attribute__ ((section (".ybss, bss, ymemory")));
/* The abcCoefficients referenced by the fooPID data structure */
/* are derived from the gain coefficients, Kp, Ki and Kd */
/* So, declare Kp, Ki and Kd in an array */
//fractional kCoeffs[] = {0,0,0};
void InitApp(void)
{
    TRISB = 0x000F;
    TRISC = 0x0000;
    TRISD = 0x0002;
    TRISE = 0x0100;
    TRISF = 0x0000;
    //_RC14=1;
   // BSTLED = 1;
   // BKLED  = 1;
   // FLTLED = 1;
  //  NORMALLED =1;
    
    
  
   _CN5IE=1;
   _CNIP=7;
   _CN5PUE=1;
   _CNIF=0;
   _CNIE=1;
    
    /* Setup analog functionality and port direction */
    ADC_Init();
    PWM_Init();
    InitTMR1();
    InitTMR3();
    Capture_Init();
    ExtINT2_Init();
    Self_Test();
    //InitPID();
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
    /* Initialize peripherals */
}

void ADC_Init(void)
{
    ADPCFG = 0xFFF8; // RB0,RB1,RB2 & RB3 = analog
ADCON1 = 0x00EC; // SIMSAM bit = 1 implies ...
// simultaneous sampling
// ASAM = 1 for auto sample after convert
// SSRC = 111 for 3Tad sample time
ADCHS = 0x0003; // Connect AN3 as CH0 input
ADCSSL = 0;
ADCON3 = 0x0302; // Auto Sampling 3 Tad, Tad = internal 2 Tcy
ADCON2 = 0x030C; // CHPS = 1x implies simultaneous ...
 IFS0bits.ADIF = 0;

        //Set the A/D interrupt enable bit
        IEC0bits.ADIE = 1;

// sample CH0 to CH3
// SMPI = 0011 for interrupt after 4 converts
ADCON1bits.ADON = 1; // turn ADC ON
}
/*
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
        ADCON2bits.SMPI = 2;
        ADCON2bits.CHPS = 3;
        ADCON2bits.BUFM = 0;
	ADCON2bits.VCFG = 0; //Ideally use external references

        //ADCON3 Register
        //We would like to set up a sampling rate of 1 MSPS
        //Total Conversion Time= 1/Sampling Rate = 125 microseconds
        //At 29.4 MIPS, Tcy = 33.9 ns = Instruction Cycle Time
        //The A/D converter will take 12*Tad periods to convert each sample
        //So for ~1 MSPS we need to have Tad close to 83.3ns
        //Using equaion in the Family Reference Manual we have
        //ADCS = 2*Tad/Tcy - 1
        ADCON3bits.SAMC = 3;
        ADCON3bits.ADCS = 4;

        //ADCHS Register
        //Set up A/D Channel Select Register to convert AN3 on Mux A input
        //of CH0 and CH1 S/H amplifiers
        ADCHS = 0x0002;

        //ADCSSL Register
        //Channel Scanning is disabled. All bits left to their default state
        ADCSSL = 0x000F;

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

}*/
//Functions:
//PWM_Init() is used to configure PWM
void PWM_Init(void)
{
    //OVDCON=0;
   //  PTPER = 682;            /* PWM period of approx. 500 nsec
 //   PTPER = PERIOD;            /* PWM period of approx. 50 usec
   //                            PWM Period = PTPER*81.38nsec = 49999.872 nsec */
    PTPER = period>>1;	// Compute Period based on CPU speed and
                                    // required PWM frequency (see defines)


    //PDC1 = PERIOD/2;             /* PWM1 pulse width of 250 nsec
     //                          Duty Cycle = PDC1*1.05nsec = 268.8 nsec */

    //PDC2 = PERIOD/2;             /* PWM2 pulse width of 250 nsec
     //                          Duty Cycle = PDC2*1.05nsec = 268.8 nsec */

//PDC3 = PERIOD/2;             /* PWM2 pulse width of 250 nsec
  //                             Duty Cycle = PDC2*1.05nsec = 268.8 nsec */
    PDC1 =0;//(2 * PTPER)*0.1;
    PDC2 =0;// (2 * PTPER)*0.1;
    //PDC1=0;
    //PDC2=0;
    //PDC3 = (2 * PTPER)*0.7;

    /* Note that a pulse appears only on every other PWM cycle. So in push-pull
       mode, the effective duty cycle is 25% */

    //Dead Time   = 81.380 ns  * 31 *1 = 2522 ns
    PWMCON1 = 0x0000;
    DTCON1bits.DTAPS = 0;
    DTCON1bits.DTA  =31;/* 33.6 nsec dead time
                               Dead-time = DTR1*1.05nsec = 33.6 nsec */

    DTCON1bits.DTBPS = 0;

    DTCON1bits.DTB  =31;/* 33.6 nsec dead time
                               Dead-time = DTR2*1.05nsec = 33.6 nsec */
    PTCONbits.PTCKPS = 0;
    PTCONbits.PTMOD = 2;
   // PTCONbits.PTCKPS = 0;
    PWMCON2 = 0x0F02;
    PWMCON1bits.PEN1L=1;
    PWMCON1bits.PEN1H=1;
    PWMCON1bits.PEN2L=1;
    PWMCON1bits.PEN2H=1;
    PWMCON1bits.PEN3L=0;
    PWMCON1bits.PEN3H=0;


//OVDCONbits.POVD1L = 0;
  //              OVDCONbits.POVD2L = 0;

OVDCONbits.POUT1L=1;
OVDCONbits.POUT1H=0;
OVDCONbits.POUT2L=1;
OVDCONbits.POUT2H=0;

//OVDCONbits.POUT3L=0;
//OVDCONbits.POUT3H=1;

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

   //PTCONbits.PTEN = 1;     /* Turn ON PWM module */
}
void Capture_Init(void)
{
 IC2CONbits.ICM = 0;
    IC2CONbits.ICTMR = 0;
    IC2CONbits.ICSIDL=0;
    IC2CONbits.ICOV=0;
    IC2CONbits.ICBNE=0;
    IC2CONbits.ICI=0;
    IC2CONbits.ICM = 1;
//IC2CON = 0x0001;
    IPC1bits.IC2IP=6;
    IFS0bits.IC2IF=0;
    IEC0bits.IC2IE=1;

  //  _IC2IP=7;
    //IC2CON = 0x0001;
    //IC2CONbits.ICSIDL =0;
   // IC2CONbits.ICTMR = 0;
   // IC2CONbits.ICI=0;
    //_IC2IF=0;
    //_IC2IE=1;
    


}
void InitTMR1(void)
{
	T1CON = 0x0000;			// internal Tcy/64 clock
	TMR1 = 0;
	PR1 = 1234;
        IPC0bits.T1IP=1;
        IFS0bits.T1IF=0;
        IEC0bits.T1IE=1;
	T1CONbits.TON = 1;		// turn on timer 3
	return;
}
void InitTMR3(void)
{
	T3CON = 0x0030;			// internal Tcy/64 clock
	TMR3 = 0;
	PR3 = 0xFFFF;
        IPC1bits.T3IP=1;
        IFS0bits.T3IF=0;
        IEC0bits.T3IE=1;
	T3CONbits.TON = 1;		// turn on timer 3
	return;
}
void Self_Test(void)
{
   
    /*for(j=0;j<5;j++)
    {
        while(!sec_chk){sec_chk=0;}
    if(((inputvoltage>>2)>=LowInVolt)&&((inputvoltage>>2)<=MaxInVolt))
    {
       for(j=0;j<10;j++)
        {
        NORMALLED=~NORMALLED;
     while(!sec_chk){sec_chk=0;}
        }
    }
    else
    {
       for(j=0;j<10;j++)
        {
        FLTLED=~FLTLED;
     while(!sec_chk){sec_chk=0;}
       }

    }
    }*/


        //for(k=0;k<2;k++)
        {
        BKLED=1;
        sec_chk=false;
     while(sec_chk==false){}

        }
    //BKLED=0;
       // for(k=0;k<2;k++)
        {
        BSTLED=1;
       sec_chk=false;
     while(sec_chk==false){}
        }
    //BSTLED=0;
        //for(k=0;k<2;k++)
        {
        OLLED=1;
        sec_chk=false;
     while(sec_chk==false){}
        }
    //NORMALLED=0;
    // for(k=0;k<2;k++)
        {
        NORMALLED=1;
        sec_chk=false;
     while(sec_chk==false){}
      }
        BKLED=0;BSTLED=0;NORMALLED=0;OLLED=0;



}


void stab(void)
{
    if(sec_chk)
    {
        sec_chk=false;
        OLLED=~OLLED;
    }
    if(sw==true)
    {
        if(sec_chk==false)
        {
        if(((inputvoltage>>SAMPLE)>=LowInVolt)&&((inputvoltage>>SAMPLE)<=MaxInVolt))
        {
             if (((inputvoltage>>SAMPLE)<=SetInVolt1)&&((inputvoltage>>SAMPLE)>=LowInVolt))
            {
                BKLED=0;
                BSTLED=1;
                NORMALLED=0;
                if(Bst_flag == false)
                {
                    PDC1=((2*PTPER)*.1);
                    PDC2=((2*PTPER)*.1);
                    pid=0;
                    propational =0;
                    integral_error = 0;
                }
                Bst_flag=true;
                bypass_chk=false;
                PWM_BstBk_chk=1;
            }
             else if (((inputvoltage>>SAMPLE)>=SetInVolt2)&&((inputvoltage>>SAMPLE)<=MaxInVolt))
            {
                BKLED=1;
                BSTLED=0;
                NORMALLED=0;
                if(Bst_flag == true)
                {
                    PDC1=((2*PTPER)*.9);
                    PDC2=((2*PTPER)*.9);
                    pid=0;
                    propational =0;
                    integral_error = 0;
                }
                Bst_flag=false;
                bypass_chk=false;
                PWM_BstBk_chk=0;
            }
            if (((inputvoltage>>SAMPLE)>=SetInVolt1)&&((inputvoltage>>SAMPLE)<=SetInVolt2))  //Normal mode
            {
                propational=0;
                integral_error=0;
                pid =0;
     
                BKLED=0;
                BSTLED=0;
                NORMALLED=1;
                bypass_chk=false;
                // PTCONbits.PTEN = 0;     /* Turn ON PWM module */
                OVDCONbits.POVD1L = 0;
                OVDCONbits.POVD2L = 0;
                
                return;
            }
        }
        else if(((inputvoltage>>SAMPLE)<=LowInVolt)||((inputvoltage>>SAMPLE)>=MaxInVolt))  //Over voltage and UNder voltage for input
        {
                if(Bst_flag)
                {
                    BKLED=~BKLED;
                  //  BSTLED=0;
                }
                else
                {
                    BSTLED=~BSTLED;
                    //BKLED=0;
                }
            NORMALLED=0;
            OLLED=1;
        //    OVDCONbits.POVD1L = 1;
          //  OVDCONbits.POVD2L = 1;
            OVDCONbits.POVD1L = 0;
                OVDCONbits.POVD2L = 0;
           
                return;
        }
        }
    }
    else    // Switch off
    {
        NORMALLED=0;
        BKLED=0;
        BSTLED=0;

        OVDCONbits.POVD1L = 0;
                OVDCONbits.POVD2L = 0;
        //PDC1 =0;
       // PDC2 = 0;
        propational=0;
                integral_error=0;
                pid =0;
               return;
    }
    //if(bypass_chk) return;
    if (((inputvoltage>>SAMPLE) >=LowInVolt)&&((inputvoltage>>SAMPLE)<=MaxInVolt))
    {
        if(dutycycle_chk)
        {
                PID_Update();
            //    PDC1=((2*PTPER)*.50);
             //   PDC2=((2*PTPER)*.50);
     
           //      if(Bst_flag==true)
                PDC1 = (((int32_t)(pid/1.5)));
       //         else
         //        PDC1 = ((2*PTPER)-(PDC1+(pid/1.5)));
               if(PDC1>((2*PTPER)*.90))
                      PDC1=((2*PTPER)*.90);
               if(PDC1<((2*PTPER)*.1))
                      PDC1=((2*PTPER)*.1);
               //PDC2=PDC2+2;//-pid;
             //   if(Bst_flag==true)
                PDC2 = (((int32_t)(pid/1.5)));
         //       else
           //      PDC2 = ((2*PTPER)-(PDC2+(pid/1.5)));
               if(PDC2>((2*PTPER)*.90))
                      PDC2=((2*PTPER)*.9);
               if(PDC2<((2*PTPER)*.1))
                      PDC2=((2*PTPER)*.1);

dutycycle_chk=0;
        }
    }
   
}
/*void PID_check(void)
{
    error= setpoint - actual;
    if(error<integrallimit)
    {
        integral = integral + error;
    }
    else
        integral = 0;
 P = error* kp;
 I = integral * ki;
 D = (prev-actual)*kd;
}*/
void ExtINT2_Init(void)
{
    INTCON2bits.INT2EP =0;
    IFS1bits.INT2IF = 0;
    IPC5bits.INT2IP = 7;
    IEC1bits.INT2IE = 1;
}
/*
void Read_ADC(void)
{
    ADC16ptr = &ADCBUF0;
    IFS0bits.ADIF=0;
    while(IFS0bits.ADIF)

        for(count = 0;count<3;count++)
        {
            inputvoltage= *ADC16ptr++;

        }
    count=0;

}*/
void Run_PWM(void)
{
    if(PTCONbits.PTEN)
PTCONbits.PTEN = 1;     /* Turn ON PWM module */

}

void PID_Update(void)
{
   //pid = 0;
   current_value = (outputvoltage>>SAMPLE);
   current_value=0;
   error =setpoint - current_value;

   if (abs(error) <1024){ // prevent integral 'windup'
      integral_error = integral_error + error; // accumulate the error integral
   }
   
   propational = error * kp;
   integral = integral_error * ki;
  // derivative = ((error-last)/0.01) * kd;

   pid =( propational + integral);// + derivative);

    
   if(pid<0)
   {
       //NORMALLED = 1;
       OLLED =0;
      
   }
   else
   {
       OLLED =1;
      // NORMALLED =0;
      
   }
 /*  if(pid > 340)
   {
     pid =340;
   }
   else if(pid < -340)
   {
       pid =-340;
   }*/
   
   last = error;
}
