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
#define BYPASSLED _RD0
#define FLTLED _RE8

#define HiIPVolt 600
#define LoIPVolt 300
#define NormalOPVolt 512
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


extern uint16_t inputvoltage;
extern uint16_t outputvoltage;
extern uint16_t outputcurrent;
extern uint32_t period;