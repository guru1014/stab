/* 
 * File:   lcd.c
 * Author: Narayanan
 *
 * Created on November 8, 2014, 12:07 PM
 */
#include "lcd.h"
#include <stdio.h>

int shift_count=0;

void delay(unsigned int j)
{
    unsigned int k=0;
                unsigned int i=0;
                for(i=0;i<j;i++)
                {
                    for(k=0;k<j;k++)
                    {
                        
                    }
                }
}

void lcd_init(void)
{
 //   int i=0;
 //   char buff[16];
    //LCD_EN=1;
   // LCD_RS=0;
  //  while(1);
    LCD_CLK=0;
    LCD_DB=0;
    LCD_RS=0;
    LCD_EN=0;
    _RB4=1;
   

    delay(lcd_delay);
    delay(lcd_delay);
    delay(lcd_delay);
    delay(lcd_delay);

     delay(lcd_delay);
    delay(lcd_delay);
    delay(lcd_delay);
    delay(lcd_delay);

   
    
_RB4=0;
   
    lcd_command(0x38);
    delay(lcd_delay);
    lcd_command(0x0C);
    delay(lcd_delay);
    lcd_command(0x01);
    delay(lcd_delay);
    lcd_command(0x06);
    delay(lcd_delay);
  //  sprintf(buff,"Hello");
 //   lcd_puts(buff);
 //   lcd_puts("Good Morning");
   
 
}
void lcd_command(unsigned char st)
{
       LCD_CLK=0;
       LCD_DB=((st>>3) & 0x01)?1:0;
       delay(shift_delay);
       LCD_CLK=1;
       delay(shift_delay);
      for(shift_count=7;shift_count>=0;shift_count--)
       {
          if(shift_count!=3)
          {
       LCD_CLK=0;
       LCD_DB=((st>>shift_count) & 0x01)?1:0;
       delay(shift_delay);
       LCD_CLK=1;
       delay(shift_delay);
          }
       }
    LCD_RS=0;
    LCD_EN=1;
    delay(lcd_delay);
    LCD_EN=0;
   //  delay(lcd_delay);
}

void lcd_data(unsigned char a)
{
       LCD_CLK=0;
       LCD_DB=((a>>3) & 0x01)?1:0;
       delay(shift_delay);
       LCD_CLK=1;
       delay(shift_delay);
      for(shift_count=7;shift_count>=0;shift_count--)
       {
          if(shift_count!=3)
          {
       LCD_CLK=0;
       LCD_DB=((a>>shift_count) & 0x01)?1:0;
       delay(shift_delay);
       LCD_CLK=1;
       delay(shift_delay);
          }
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
