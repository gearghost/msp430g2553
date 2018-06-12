/*
 * main.c
 */
/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
#include "stdio.h"
#include "lcd1602.h"


char *F2S(double d, char* str)
{
  char str1[40];
  int j=0,k,i;
  i = (int)d;  //浮点数的整数部分
  //d = d-(int)d;
  while(i>0)
  {
    str1[j++] = i%10+'0';
    i /= 10;
  }
  for(k=0;k<j;k++)
    str[k] = str1[j-1-k]; //
  str[j++] = '.';
  d -= (int)d;
  for(i=0;i<10;i++)
  {
    d *= 10;
    str[j++] = (int)d+'0';
    d -= (int)d;
  }
  while(str[--j]=='0');
    str[++j] = '\0';
  return str;
}


void main(void) {
	WDTCTL=WDTPW+WDTHOLD;
	char str[16];
	LCD_init();
	delay_1ms();
	LCD_write_string(0,0,F2S((double)3.14159,str));
	delay_1ms();
	while(1);
}
