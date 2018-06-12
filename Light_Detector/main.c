/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
#include "LQ12864.h"

#define CPU_F 1000000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delaynus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))

#define LED_ON P1OUT|=BIT6
#define LED_OFF P1OUT&=~BIT6

void GPIO_Init(void){
	P1DIR|=BIT6;
	P1OUT&=~BIT6;
	P2DIR|=BIT2+BIT3+BIT4+BIT5;
}

unsigned int ad[4];



void main(void) {
	WDTCTL=WDTPW+WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	GPIO_Init();
	LCD_Init();
	ADC10CTL1=INCH1+CONSEQ_3;
	ADC10CTL0=ADC10SHT_2+MSC+ADC10ON+ADC10IE;
	ADC10AE0=BIT0+BIT1;
	ADC10DTC1=0x04;
	LCD_P6x8Str(0,0,"Brightness:");
	while(1){
		ADC10CTL0&=~ENC;
		while(ADC10CTL1&BUSY);
		ADC10SA=(unsigned int)ad;
		ADC10CTL0|=ENC+ADC10SC;
		__bis_SR_register(CPUOFF+GIE);
		delaynus(100);
		if(ad[1]>600)LED_ON;
		else LED_OFF;
		Display_NRF24L01((u32)ad[1]);
	}
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void){
	__bic_SR_register_on_exit(CPUOFF);
}
