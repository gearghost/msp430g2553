/*
 * main.c
 */
#include <msp430g2553.h>

#define CPU_F 1000000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delaynus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))

#define TRIG BIT3
#define US_OUT P2OUT
#define US_DIR P2DIR
#define ECHO BIT4;
#define US_IE P2IE
#define US_IES P2IES
#define US_SEL P2SEL

unsigned int count_wide;
unsigned int count_start;
unsigned int count_end;

double count_time;
double distance;
char str[16];

void buzzer(void){
	int i=0;
	while(i<3){
	P2OUT|=BIT5;
	delaynms(300);
	P2OUT&=~BIT5;
	delaynms(300);
	i++;
	}
}


void main(void) {
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P2DIR|=BIT5;
	US_DIR|=TRIG;
	US_OUT|=TRIG;
	US_SEL|=ECHO;
	TA1CTL=TASSEL_2+MC_2;
	TA1CCTL2=CAP+CM_3+CCIS_0+SCS+CCIE;
	_EINT();
	delaynms(1);
	while(1){
		US_OUT|=TRIG;
		delaynus(24);
		US_OUT&=~TRIG;
		delaynms(25);
		count_time=1.00000e-6*count_wide;
		distance=count_time*340/2.0;
		delaynms(200);
		if(distance<0.5)buzzer();
		delaynms(100);

	}
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A(void){
	switch(TA1IV){
	case 2:break;
	case 4:
		if(TA1CCTL2&CCI)
			count_start=TA1CCR2;
		else
			count_end=TA1CCR2;
			count_wide=count_end-count_start;
			break;
	case 10:
		break;
	}
}
