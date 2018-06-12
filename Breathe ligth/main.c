#include <msp430g2553.h>
#include <intrinsics.h>
#include "stdio.h"
#include "lcd1602.h"

#define CPU_F 1100000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))

#define TRIG BIT3
#define US_OUT P2OUT
#define US_DIR P2DIR
#define ECHO BIT4;
#define US_IE P2IE
#define US_IES P2IES
#define US_SEL P2SEL

unsigned int pwm_wide;
unsigned int pwm_start;
unsigned int pwm_end;

double pwm_time;
double  distance;
char str[16];

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


void main()
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog
LCD_init();
US_DIR|=TRIG;
US_OUT|=TRIG;
US_SEL|=ECHO;
TA1CTL=TASSEL_2+MC_2;
TA1CCTL2=CAP+CM_3+CCIS_0+SCS+CCIE;
_EINT();
delay_1ms();
LCD_write_string(3,0,"Distance:(m)");
delay_1ms();
while(1){
	/*TA1CTL|=MC_2+TASSEL_2+TACLR;
	TA1CCTL2|=CM_1+CAP+SCS+CCIE+CCIS_0;*/
	US_OUT|=TRIG;
	__delay_cycles(2000);
	US_OUT&=~TRIG;
	/*while((TA1CCTL2&CCIFG)==0);
	TA1CTL&=~MC_3;
	TA1CTL|=MC_2+TACLR;
	TA1CCTL2&=~CCIFG;
	TA1CCTL2|=CM_2;
	while((TA1CCTL2&CCIFG)==0);
	capV=TA1CCR2;
	distance=1.12826e-3*capV;
	sprintf(str,"%.3f",distance);
	LCD_write_string(5,1,str);
	TA1CCTL2&=~CCIFG;*/
	unsigned int x,y;
	for(x=5000;x>0;x--)
	for(y=5000;y>0;y--);
	pwm_time=1.12826e-6*pwm_wide;
	distance=pwm_time*340/2.0;
	delay_1ms();
	LCD_write_string(0,1,F2S((double)distance,str));
	delaynms(200);
}
}


#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A(void){
	switch(TA1IV){
	case 2:break;
	case 4:
		if(TA1CCTL2&CCI)
			pwm_start=TA1CCR2;
		else
			pwm_end=TA1CCR2;
			pwm_wide=pwm_end-pwm_start;
			break;
	case 10:
		break;
	}
}


