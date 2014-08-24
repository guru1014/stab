/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
//#define FCY  12288000	 // xtal = 6.144Mhz; PLLx8
//#define SYS_FREQ        491520000L
//#define FCY1             SYS_FREQ/4

//#define FPWM 20000L		 // 20 kHz, so that no audible noise is present.

//#define PERIOD  614   // 20 KHz
//#define PERIOD  682   // 18 KHz
#define BKLED _RB4
#define BSTLED  _RB5
#define NORMALLED _RD0
#define OLLED _RC13
#define FLTLED _RE8

//#define SW _RB3
#define Buzzer _RC14
#define HiIPVolt 600
#define LoIPVolt 300
#define NormalOPVolt 512

// ADC count values
#define MaxInVolt 580                          // 614 - 2.85 V, 634 - 2.95 v
#define LowInVolt 200                            //  205 - 1.0 v ,165 - 0.8 v


#define SetInVolt1 445
#define SetInVolt2 480

#define MaxOutVolt 501
#define LowOutVolt  420
#define SetOutVolt 470

#define SAMPLE 16
#define BUZZER _RC14

#define OverLoad 614
/* TODO Application specific user parameters used in user.c may go here */
/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
void stab(void);
/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void); /* I/O and Peripheral Initialization */
void Run_PWM(void);
void Read_ADC(void);
void Capture_Init(void);
void ExtINT2_Init(void);


extern uint32_t inputvoltage;
extern uint32_t outputvoltage;
extern uint32_t outputcurrent;
extern uint32_t period;
extern volatile bool dutycycle_chk;
extern volatile bool sec_chk;
extern unsigned char PWM_BstBk_chk;
extern bool sw;
extern bool bypass_chk;

