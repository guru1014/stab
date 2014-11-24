/* 
 * File:   lcd.h
 * Author: Narayanan
 *
 * Created on November 8, 2014, 1:15 PM
 */

#include <p30Fxxxx.h>        /* Device header file */
#define LCD_RS LATCbits.LATC14
#define LCD_EN LATCbits.LATC13

#define LCD_DB LATEbits.LATE4
#define LCD_CLK LATEbits.LATE5
//#define LCD_DB5 _RF2
//#define LCD_DB4 _RF3

#define lcd_delay 50
#define shift_delay 50





void delay(unsigned int j);
void lcd_init(void);
void lcd_command(unsigned char st);
void lcd_data(unsigned char a);
void lcd_puts(char *str);


