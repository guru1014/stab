/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p30Fxxxx.h>        /* Device header file                            */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include <dsp.h>
#include <libq.h>
#include "user.h"            /* variables/params used by user.c               */
#include "lcd.h"

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

#define DELAYCOUNT 25
//void Read_ADC(void);
/* TODO Initialize User Ports/Peripherals/Project here */
const uint16_t *ADC16ptr;
uint16_t count;
int k=0;
bool Bst_flag=false;
//
float error;
float current_value=0,last=0;
float setpoint=470;
float integral_error=0;
float propational=0;
float integral=0;
float derivative = 0;
float kp=0.05;
float ki=0.05;
float kd=0;
float pid=0;
unsigned int normalcount=DELAYCOUNT;
unsigned int bstcount=DELAYCOUNT;
unsigned int bkcount=DELAYCOUNT;
unsigned int overcount =DELAYCOUNT;
unsigned int duty=0;
unsigned int dcou=0,ddelay=0;
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
    _RE8=1;
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
    lcd_init();
    display(0);
    Self_Test();
    //InitPID();
    if(_RB3==0)
    {
        if(_RB3==0)
        {
           // NORMALLED=1;
            sw=true;
          //  BUZZER=0;
            PTCONbits.PTEN = 1;
        }
    }
    else
    {
        if(_RB3==1)
        {
         //   NORMALLED=0;
            sw=false;
          //  BUZZER=1;

            PTCONbits.PTEN = 1;
        }
    }
   
    lcd_command(0x01);
   
  //  sw=true;PTCONbits.PTEN = 1;
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
ADCON3 = 0x0A08; // Auto Sampling f Tad, Tad = internal 2 Tcy
ADCON2 = 0x030C; // CHPS = 1x implies simultaneous ...
 IFS0bits.ADIF = 0;

        //Set the A/D interrupt enable bit
        IEC0bits.ADIE = 1;

// sample CH0 to CH3
// SMPI = 0011 for interrupt after 4 converts
ADCON1bits.ADON = 1; // turn ADC ON
}

//Functions:
//PWM_Init() is used to configure PWM
void PWM_Init(void)
{
 
    PTPER = period>>1;	// Compute Period based on CPU speed and
    PDC1 =0;//(2 * PTPER)*0.1;
    PDC2 =0;// (2 * PTPER)*0.1;
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


   _FLTAIP=7;
    _FLTAIF=0;
    _FLTAIE=1;
    FLTACON = 0x0F;
     OVDCONbits.POVD1L = 0;
     OVDCONbits.POVD2L = 0;

OVDCONbits.POUT1L=1;
OVDCONbits.POUT1H=0;
OVDCONbits.POUT2L=1;
OVDCONbits.POUT2H=0;

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
       // OLLED=1;
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
        BKLED=0;BSTLED=0;NORMALLED=0;//OLLED=0;



}


void stab(void)
{
    if(sec_chk)
    {
        sec_chk=false;
        //OLLED=~OLLED;
        if(++ddelay==1)
        {
            ddelay=0;
       // display(++dcou);
            display(4);
        if(dcou==2)dcou=0;
        }
    }
    if(sw==true)
    {
        if(sec_chk==false)
        {
        if(((inputvoltage>>avgcou)>=LowInVolt)&&((inputvoltage>>avgcou)<=MaxInVolt))
        {
             if (((inputvoltage>>avgcou)<=SetInVolt1)&&((inputvoltage>>avgcou)>=LowInVolt))
            {
                 bstcount--;
                 if(bstcount==0)
                 {
                     bstcount =1;
                     normalcount =DELAYCOUNT;
                     bkcount=DELAYCOUNT;
                     overcount=DELAYCOUNT;
                BKLED=0;
                BSTLED=1;
                NORMALLED=0;
                if(Bst_flag == false)
                {
                    PDC1=((2*PTPER)*0.0);
                    PDC2=((2*PTPER)*0.0);
                    pid=0;
                    propational =0;
                    integral_error = 0;
                }
                Bst_flag=true;
                bypass_chk=false;
                PWM_BstBk_chk=1;
                 }
            }
             else if (((inputvoltage>>avgcou)>=SetInVolt2)&&((inputvoltage>>avgcou)<=MaxInVolt))
            {
                 bkcount--;
                 if(bkcount==0)
                 {
                     bkcount =1;
                     normalcount =DELAYCOUNT;
                     bstcount=DELAYCOUNT;
                     overcount=DELAYCOUNT;

                BKLED=1;
                BSTLED=0;
                NORMALLED=0;
                if(Bst_flag == true)
                {
                    PDC1=((1*PTPER)*0);
                    PDC2=((1*PTPER))*0;
                    pid=0;
                    propational =0;
                    integral_error = 0;
                }
                Bst_flag=false;
                bypass_chk=false;
                PWM_BstBk_chk=0;
                 }
            }
             /*else if (((inputvoltage>>avgcou)>=SetInVolt1)&&((inputvoltage>>avgcou)<=SetInVolt2))  //Normal mode
            {
                normalcount--;
                 if(normalcount==0)
                 {
                     normalcount =1;
                     bstcount =DELAYCOUNT;
                     bkcount=DELAYCOUNT;
                     overcount=DELAYCOUNT;
                propational=0;
                integral_error=0;
                pid =0;
     
                BKLED=0;
                BSTLED=0;
                NORMALLED=1;
                bypass_chk=false;
                // PTCONbits.PTEN = 0;     
                //OVDCONbits.POVD1L = 0;
                //OVDCONbits.POVD2L = 0;
                //return;
                 //}
                

            //}*/
        }
        else if(((inputvoltage>>avgcou)<=LowInVolt)||((inputvoltage>>avgcou)>=MaxInVolt))  //Over voltage and UNder voltage for input
        {
                overcount--;
                 if(overcount==0)
                 {
                     overcount =1;
                     bstcount =DELAYCOUNT;
                     bkcount=DELAYCOUNT;
                     normalcount=DELAYCOUNT;
                     BSTLED=1;
                     BKLED=1;
                     NORMALLED=1;
             //   if(Bst_flag)
                {
               //     BKLED=~BKLED;
                  //  BSTLED=0;
                }
                //else
                {
                  //  BSTLED=~BSTLED;
                    //BKLED=0;
                }
                      propational=0;
                integral_error=0;
                pid =0;
     
            //NORMALLED=0;
          //  OLLED=1;
        //    OVDCONbits.POVD1L = 1;
          //  OVDCONbits.POVD2L = 1;
            OVDCONbits.POVD1L = 0;
            OVDCONbits.POVD2L = 0;
           
           //PDC1=((1.4*PTPER));
           //PDC2=((1.4*PTPER));
            return;
                 }
                //return;
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
    if (((inputvoltage>>avgcou) >=LowInVolt)&&((inputvoltage>>avgcou)<=MaxInVolt))
    {
        //if(dutycycle_chk)
        {
       //    PORTBbits.RB5=1;
                PID_Update();
            //    PDC1=((2*PTPER)*.50);
             //   PDC2=((2*PTPER)*.50);
              //  NORMALLED=~NORMALLED;
                 if(Bst_flag==true)
                 {
                     if(pid<=0){
                         pid=0;
                    //     integral_error=0;
                      //   propational=0;
        //                 NORMALLED=1;
                     }
                duty = (((int32_t)(pid/1.5)));
                if(duty>=((2*PTPER)*1))
                      duty=((2*PTPER)*1);

                 }
                else
                {
                 //duty = (int32_t)((2*PTPER)-(pid/1.5));
                     if(pid>0)
                     {
                        // pid =-((1*PTPER)*1.5);
                         pid=0;
    //                     integral_error=0;
      //                   propational=0;
      //                   NORMALLED=1;
                     }
                     duty = (((int32_t)(-pid/1.5)));
                 if(duty>=((1*PTPER)))
                      duty=((1*PTPER));

                }
               if(duty<=((2*PTPER)*0.0))
                      duty=((2*PTPER)*0.0);
               //PDC2=PDC2+2;//-pid;
          //      if(Bst_flag==true)
            //    PDC2 = (((int32_t)(pid/1.5)));
             // else
               //  PDC2 = (int32_t)((2*PTPER)-(pid/1.5));
              // if(PDC2>((2*PTPER)*.95))
               //       PDC2=((2*PTPER)*.98);
               //if(PDC2<((2*PTPER)*.05))
                 //     PDC2=((2*PTPER)*.02);
                PDC1=duty;
                PDC2=duty;
dutycycle_chk=0;
//PORTBbits.RB5=0;
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
   current_value = (outputvoltage>>avgcou1);
   //current_value=200;
   error =setpoint - current_value;

   if (abs(error) <1024){ // prevent integral 'windup'
      integral_error = integral_error + error; // accumulate the error integral
   }
   else
       integral_error=0;

   propational = error * kp;
   integral = integral_error * ki;
  // derivative = ((error-last)/0.01) * kd;

   pid =( propational + integral);// + derivative);

    
   if(pid<=0)
   {
       NORMALLED = 1;
       //OLLED =0;
      
   }
   else
   {
       //OLLED =1;
       NORMALLED =0;
      
   }
   if(pid >=((2*PTPER)*1.5))
   {
     pid =((2*PTPER)*1.5);
   }
   else if(pid <= -((2*PTPER)*1.5))
   {
       pid =-((2*PTPER)*1.5);
   }
   
   last = error;
}

void display(unsigned int dispcou)
{
    switch(dispcou)
    //switch(3)
    {
        default:
        case 0:lcd_displayname("Velsine         ",0x80);
              lcd_displayname("StaticStabilizer",0xC0);break;
        case 1:lcd_displayname("Input Voltage ",0x80);
              lcd_parameter((inputvoltage>>avgcou),0xc0,'V');break;
        case 2:lcd_displayname("Output Voltage",0x80);
              lcd_parameter((outputvoltage>>avgcou),0xc0,'V');break;
        case 3:lcd_displayname("Load Current  ",0x80);
              lcd_parameter((outputcurrent>>avgcou),0xc0,'A');break;
        case 4:lcd_parameter((inputvoltage>>avgcou),0x80,'V');
              lcd_parameter((outputvoltage>>avgcou1),0xc0,'V');break;

        
    }
}
void lcd_displayname(char *str1,unsigned char lineno)
{
    lcd_command(0x02);
    lcd_command(lineno);
    lcd_puts(str1);
}
void lcd_parameter(uint32_t parameter,unsigned char lineno,unsigned char unit)
{
  //  PORTBbits.RB5=~PORTBbits.RB5;
    char s1[8];
    uint32_t tmp;
    s1[5]=' ';
    s1[6]=unit;
    s1[7]=0;
    tmp=(uint32_t)parameter*488.828125;
   // tmp = check_cou;
    tmp=tmp/100;
    s1[4]=((long)tmp%10)+48;
   // tmp=tmp/10;
    s1[3]='.';
    tmp=tmp/10;
    s1[2]=((long)tmp%10)+48;
    tmp=tmp/10;
    s1[1]=((long)tmp%10)+48;
    tmp=tmp/10;
    s1[0]=((long)tmp%10)+48;
    //  s1[6]=0;
   /* tmp=(uint32_t)(outputvoltage>>avgcou)*0.48828;
   // tmp = check_cou;
    s1[5]=((long)tmp%10)+48;
    tmp=tmp/10;
    s1[4]=((long)tmp%10)+48;
    tmp=tmp/10;
    s1[3]=((long)tmp%10)+48;
    tmp=tmp/10;
    s1[2]=((long)tmp%10)+48;
    tmp=tmp/10;
    s1[1]=((long)tmp%10)+48;
    tmp=tmp/10;
    s1[0]=((long)tmp%10)+48;*/
    //lcd_command(0x01);
    lcd_command(lineno);
    lcd_puts(s1);
}