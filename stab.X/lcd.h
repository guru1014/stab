/* 
 * File:   lcd.h
 * Author: Narayanan
 *
 * Created on November 8, 2014, 1:15 PM
 */

#include <p30Fxxxx.h>        /* Device header file */
#define LCD_RS PORTCbits.RC13
#define LCD_EN PORTCbits.RC14

#define LCD_DB PORTEbits.RE4
#define LCD_CLK PORTEbits.RE5
//#define LCD_DB5 _RF2
//#define LCD_DB4 _RF3

#define lcd_delay 400
#define shift_delay 100


void delay(unsigned int j);
void lcd_init(void);
void lcd_init_write(unsigned char a);
void lcd_command(unsigned char a);
void lcd_data(unsigned char a);
void lcd_puts(char *str);


