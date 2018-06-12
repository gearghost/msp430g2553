/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
/*#include "lcd1602.h"*/

#define CPU_F 1000000
#define delay_nus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))
#define delay_nms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))


void Init_HC06() {
P1SEL = BIT1+BIT2; P1SEL2 = BIT1+BIT2;
UCA0CTL1 |= UCSSEL_2;
UCA0BR0 = 106;
UCA0BR1 = 0;
UCA0MCTL = UCBRS2 + UCBRS0;
UCA0CTL1 &=~ UCSWRST;
IE2|=UCA0RXIE;
}

void SendChar(unsigned char c)
{
while(!(IFG2&UCA0TXIFG));
UCA0TXBUF=c;
IFG2&=~UCA0RXIFG;
}

unsigned char RevChar()
{
unsigned char c;
while(!(IFG2&UCA0RXIFG));
c=UCA0RXBUF;
IFG2 &=~ UCA0TXIFG;
UCA0RXBUF=0;
return c;
}


volatile unsigned char temp='\0';
char s[16]={0};

void main(void)
{
	WDTCTL=WDTPW+WDTHOLD;
	P1DIR|=BIT3+BIT4;
	P2DIR|=BIT0+BIT1;
	P2OUT&=~BIT0+BIT1;
	P1OUT&=~BIT3+BIT4;
	/*LCD_init();*/
	Init_HC06();
	/*LCD_write_string(0,0,"BT-COMMUNICATION");*/
	while(1){
		temp=RevChar();
		s[0]=temp;
		delay_nms(1);
		if(s[0]=='A')P1OUT|=BIT3;
		if(s[0]=='B')P1OUT|=BIT4;
		if(s[0]=='C')P2OUT|=BIT1;
		if(s[0]=='D')P2OUT|=BIT0;
		/*LCD_write_string(0,1,s);*/
		delay_nms(2000);
		P1OUT&=~BIT3+BIT4;
		P2OUT&=~BIT0+BIT1;
	}
}

