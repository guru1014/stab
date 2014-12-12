/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p30Fxxxx.h>      /* Device header file */

/******************************************************************************/
/* Configuration Bits                                                         */
/*                                                                            */
/* This is not all available configuration bits for all dsPIC devices.        */
/* Refer to the dsPIC device specific .h file in the compiler                 */
/* support\dsPIC30F\h directory for complete options specific to the device   */
/* selected.  For additional information about what the hardware              */
/* configurations mean in terms of device operation, refer to the device      */
/* datasheet 'Special Features' chapter.                                      */
/*                                                                            */
/******************************************************************************/

/* TODO Fill in your configuration bits here and remove the #if 0. */

/* The general style is below: */

#if 1

_FOSC(XT_PLL8 & CSW_FSCM_OFF);
_FBORPOR(PWRT_16 & BORV_45 & PBOR_ON & MCLR_EN & RST_PWMPIN ); /* Ext MCLR, no brownout ,PWM H L low*/
_FWDT(WDT_OFF); /* Turns off watchdog timer */
//_ICD(ICS_PGD1); /* Selects debug channel channel */
_FGS(CODE_PROT_OFF);

#endif
