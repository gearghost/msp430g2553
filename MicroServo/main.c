/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>

#define CPU_F 1000000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delaynus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))


void Timer_A0_1_init(){
	TACTL|=TASSEL_1+MC_1;
	CCTL1=OUTMOD_7;
	CCR0=100;
	CCR1=50;
}

void Timer_A1_2_init(){
	TA1CTL|=TASSEL_1+MC_1;
	TA1CCTL2=OUTMOD_7;
	TA1CCR0=10000;
	TA1CCR2=7000;
}

void IO_init(){
	P1SEL|=BIT6;
	P1DIR|=BIT6;
	P2SEL|=BIT5;
	P2DIR|=BIT5;
}


void main(void) {
	WDTCTL=WDTPW+WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P1DIR|=BIT3+BIT4+BIT7+BIT5;
	P1OUT|=BIT3;
	P1OUT&=~BIT4;
	P1OUT|=BIT5;
	P1OUT&=~BIT7;
	IO_init();
	Timer_A0_1_init();
	Timer_A1_2_init();
	_BIS_SR(CPUOFF);
}
