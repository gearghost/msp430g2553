/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
#include "lcd1602.h"

#define CPU_F 1100000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delaynus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))

#define ADDT BIT3
#define ADSK BIT4
#define AD_DIR P2DIR
#define AD_OUT P2OUT
#define AD_IN P2IN
#define ADSK_1 AD_OUT|=ADSK
#define ADSK_0 AD_OUT&=~ADSK
#define ADDO (AD_IN&ADDT)

#define WEIGHT_BASE 32420
unsigned char Data_str[16];


void HX711_Init(void){
	AD_DIR|=ADSK;
	AD_DIR&=~ADDT;
}

unsigned int ReadCount(void){
	unsigned long Count;
	unsigned int ADval;
	unsigned char i;
	ADSK_0;
	Count=0;
	while(ADDO);
	for(i=0;i<24;i++){
		ADSK_1;
		Count=Count<<1;
		ADSK_0;
		if(ADDO)Count++;
	}
	ADSK_1;
	Count=Count^0x800000;
	ADSK_0;
	ADval=(unsigned int)(Count>>8);
	return ADval;
}

char *WeightToString(float d, char* str)
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
  for(i=0;i<2;i++)
  {
    d *= 10;
    str[j++] = (int)d+'0';
    d -= (int)d;
  }
  while(str[--j]=='0');
    str[++j] = '\0';
  return str;
}

unsigned long AD_val;
float weight;

void main(void) {
	WDTCTL=WDTHOLD+WDTPW;
	LCD_init();
	HX711_Init();
	delaynms(1);
	LCD_write_string(2,0,"ELE SCALE (g)");
	delaynms(1);
	while(1){
		AD_val=ReadCount();
		weight=0.61*(WEIGHT_BASE-AD_val);
		LCD_write_string(4,1,WeightToString(weight,Data_str));
	}
}
