/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>

#define CPU_F 1100000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))

typedef unsigned char uchar;

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

void Moto_Driver_Init(void){
	P1DIR|=BIT0+BIT3+BIT4+BIT5+BIT6+BIT7;
	P1OUT&=~BIT0+BIT3+BIT4+BIT5+BIT6+BIT7;
}

void Start(void){
	P1OUT|=BIT6;
	P1OUT|=BIT0;
}

void Stop(void){
	P1OUT&=~BIT6;
	P1OUT&=~BIT0;
}


void Turn_Left(void){
	P1OUT|=BIT4;
	P1OUT&=~BIT3;
	P1OUT&=BIT7;
	P1OUT|=BIT5;
	delaynms(1000);
	P1OUT|=BIT7;
	P1OUT&=~BIT5;
}

void Turn_Right(void){
	P1OUT|=BIT7;
	P1OUT&=~BIT5;
	P1OUT&=~BIT4;
	P1OUT|=BIT3;
	delaynms(1000);
	P1OUT|=BIT4;
	P1OUT&=~BIT3;
}

void Forward(void){
	P1OUT|=BIT7;
	P1OUT&=~BIT5;
	P1OUT|=BIT4;
	P1OUT&=~BIT3;
}


void Backward(void){
	P1OUT&=~BIT7;
	P1OUT|=BIT5;
	P1OUT&=BIT4;
	P1OUT|=BIT3;
}


void Left_Round(void){
	P1OUT|=BIT6;
	P1OUT&=~BIT0;
}

void Right_Round(void){
	P1OUT&=BIT6;
	P1OUT|=BIT0;
}



void main(void) {
	WDTCTL=WDTPW+WDTHOLD;
	Init_HC06();
	Moto_Driver_Init();
	while(1){
		temp=RevChar();
		if(temp=='S')Start();
		else if(temp=='s')Stop();
		else if(temp=='l')Turn_Left();
		else if(temp=='r')Turn_Right();
		else if(temp=='f')Forward();
		else if(temp=='b')Backward();
		else if(temp=='L')Left_Round();
		else if(temp=='R')Right_Round();
	}
}
