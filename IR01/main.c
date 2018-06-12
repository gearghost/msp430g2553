/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
#include "lcd1602.h"
#define CPU_F 1100000
#define delaynms(x)  __delay_cycles((long)(CPU_F*(double)x/1000.0))

int counter=0;
char Alert_Counter[16];

char *IntToString(int i,char *str){
	int a,b,c,d;
	a=i/1000;
	b=(i-a*1000)/100;
	c=(i-a*1000-b*100)/10;
	d=(i-a*1000-b*100-c*10);
	str[0]=a+0x30;
	str[1]=b+0x30;
	str[2]=c+0x30;
	str[3]=d+0x30;
	return str;
}


void buzzer(void){
	int i=0;
	while(i<3){
	P2OUT|=BIT4;
	P2OUT|=BIT3;
	delaynms(300);
	P2OUT&=~BIT4;
	P2OUT&=~BIT3;
	delaynms(300);
	i++;
	}
}

char *floatToString(float d, char* str)
{
  char str1[40];
  int j=0,k,i;
  i = (int)d;
  while(i>0)
  {
    str1[j++] = i%10+'0';
    i /= 10;
  }
  for(k=0;k<j;k++)
    str[k] = str1[j-1-k]; //
  str[j++] = '.';
  d -= (int)d;
  for(i=0;i<3;i++)
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
	WDTCTL=WDTHOLD+WDTPW;
	LCD_init();
	P2REN|=BIT2;
	P2DIR|=BIT4+BIT3;
	P2OUT&=~BIT2;
	P2OUT&=~(BIT4+BIT3);
	delaynms(1000);
	LCD_write_string(1,0,"Alert Counter:");
	while(1){

		if(P2IN&BIT2){
			_NOP();
		}
		else {

			delay_nms(1000);
			buzzer();
			counter++;
			delaynms(10);
			LCD_write_string(6,1,IntToString(counter,Alert_Counter));
			delaynms(1);
		}
	}
}

