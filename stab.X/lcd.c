/* 
 * File:   lcd.c
 * Author: Narayanan
 *
 * Created on November 8, 2014, 12:07 PM
 */
#include "lcd.h"

unsigned int shift_count=0;

void delay(unsigned int j)
{
                unsigned int i=0;
                for(i=0;i<j;i++);
}

void lcd_init(void)
{
    lcd_command(0x38);
    delay(lcd_delay);
    lcd_command(0x0E);
    delay(lcd_delay);
    lcd_command(0x01);
    delay(lcd_delay);
    lcd_command(0x06);
    delay(lcd_delay);

              /*  delay(lcd_delay);
                lcd_init_write(0x30);   //Special Sequence:Write Function Set.
                delay(lcd_delay);
                lcd_init_write(0x30);              //Special Sequence:Write Function Set.
                delay(lcd_delay);
                lcd_init_write(0x30);              //Special Sequence:Write Function Set.
                delay(lcd_delay);
                lcd_init_write(0x20);   // 0x20 for 4-bit
                delay(lcd_delay);
                lcd_command(0x28);         //Display Off, Cursor Off, Blink Off
                delay(lcd_delay);
                lcd_command(4);                                                // Clear Screen & Returns the Cursor Home
                delay(lcd_delay);
                lcd_command(0x85);
                delay(lcd_delay);
                lcd_command(6);           //Inc cursor to the right when writing and don?t shift screen
                delay(lcd_delay);
                lcd_command(1);
                delay(lcd_delay);
                */
}


void lcd_init_write(unsigned char a)
{
    for(shift_count=8;shift_count<0;shift_count--)
       {
       LCD_CLK=0;
       LCD_DB=((a>>shift_count) & 0x01)?1:0;
       LCD_CLK=1;
       delay(shift_delay);
       }
    LCD_RS=0;
    LCD_EN=1;
    delay(lcd_delay);
    LCD_EN=0;
}

void lcd_command(unsigned char a)
{
    for(shift_count=8;shift_count<0;shift_count--)
       {
       LCD_CLK=0;
       LCD_DB=((a>>shift_count) & 0x01)?1:0;
       LCD_CLK=1;
       delay(shift_delay);
       }
    LCD_RS=0;
    LCD_EN=1;
    delay(lcd_delay);
    LCD_EN=0;
}

void lcd_data(unsigned char a)
{
    for(shift_count=8;shift_count<0;shift_count--)
       {
       LCD_CLK=0;
       LCD_DB=((a>>shift_count) & 0x01)?1:0;
       LCD_CLK=1;
       delay(shift_delay);
       }
    LCD_RS=1;
    LCD_EN=1;
    delay(lcd_delay);
    LCD_EN=0;
}
void lcd_puts(char *str)
{
 unsigned int i=0;
 for(;str[i]!=0;i++)
          lcd_data(str[i]);
}
