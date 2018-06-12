/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
#include "LQ12864.h"

#define CPU_F 1000000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delaynus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))


void GPIO_Init(void){
	P1DIR|=BIT3+BIT4+BIT5;
	P2DIR|=BIT0;
	P1DIR|=BIT7;
}

void buzzer(void){
	int i=0;
	while(i<3){
	P2OUT|=BIT3;
	delaynms(300);
	P2OUT&=~BIT3;
	delaynms(300);
	i++;
	}
}

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

volatile unsigned char temp='\0';
char s[16]={0};

unsigned int ad[2];
char str[16];
#define LED_OFF P1OUT|=BIT7
#define LED_ON P2OUT&=~BIT7

void main(void) {
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	ADC10CTL1=INCH1+CONSEQ_3;
	ADC10CTL0=ADC10SHT_2+MSC+ADC10ON+ADC10IE;
	ADC10AE0=BIT0;
	ADC10DTC1=0x02;
	GPIO_Init();
	Init_HC06();
	LCD_Init();
	LCD_P6x8Str(0,0,"PM2.5:");
	float calcVoltage = 0;
	float dustDensity = 0;
	while(1){
		temp=RevChar();
		s[0]=temp;
		delaynms(1);
		if(s[0]=='A'){
		LED_ON;
		delaynms(280);
		ADC10CTL0&=~ENC;
		while(ADC10CTL1&BUSY);
		ADC10SA=(unsigned int)ad;
		ADC10CTL0|=ENC+ADC10SC;
		delaynms(40);
		LED_OFF;
		delaynms(96);
		__bis_SR_register(CPUOFF+GIE);
		calcVoltage = ad[0] * (5.0 / 1024.0);
		dustDensity = 0.17 * calcVoltage - 0.1;
		if(dustDensity>1050)buzzer();
		LCD_P6x8Str(0,3,floatToString(dustDensity,str));
		}
		else{

		}
	}
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void){
	__bic_SR_register_on_exit(CPUOFF);
}

