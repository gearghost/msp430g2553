/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
#include "lcd1602.h"

#define CPU_F 1100000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))

typedef unsigned char uchar;

void Red_Light(void){
	int i=0;
	while(i<3){
	P1OUT|=BIT3;
	delaynms(300);
	P1OUT&=~BIT3;
	delaynms(300);
	i++;
	}
}

void Green_Light(void){
	int i=0;
	while(i<3){
	P1OUT|=BIT0;
	delaynms(300);
	P1OUT&=~BIT0;
	delaynms(300);
	i++;
	}
}

void Key_Init(void){
	P2DIR&=~BIT0+BIT1+BIT2+BIT3+BIT4;
	P2REN|=BIT0+BIT1+BIT2+BIT3+BIT4;
	P2OUT|=BIT0+BIT1+BIT2+BIT3+BIT4;
}

uchar iPassword[16]={0};
uchar New_Password[16]={0};
uchar mark_star[16]={0};
uchar pw_count=0;
uchar star_count=0;
uchar c[16];


void Read_Seg(uchar *addr,uchar *data,uchar len){
	uchar i;
	for(i=0;i<len;i++){
		*(data++)=*(addr++);
	}
}


void Read_Password(uchar *pwd){
	Read_Seg((uchar *)0x1080,pwd,4);
}

void Password_Validation(uchar *PassWord){
	uchar i;
	if(pw_count>0){
	for(i=0;i<4;i++){
		if(iPassword[i]!=PassWord[i]){
			delay_nms(10);
			LCD_write_string(0,0,"INFO:    Fail!   ");
			delay_nms(10);
			Red_Light();
			return;
		}
	}
	}
	else return;
	delay_nms(10);
	LCD_write_string(0,0,"INFO: UNLOCK! ");
	delay_nms(10);
	Green_Light();
}


void Key_Press(void){
			if(!(P2IN&BIT0)){
				delaynms(500);
			    iPassword[pw_count++]='1';
				mark_star[star_count++]='*';
				delaynms(10);
				LCD_write_string(0,1,mark_star);
				delaynms(10);

			}

			if(!(P2IN&BIT1)){
				delaynms(500);
				iPassword[pw_count++]='2';
				mark_star[star_count++]='*';
				delaynms(10);
				LCD_write_string(0,1,mark_star);

			}

			if(!(P2IN&BIT2)){
				delaynms(500);
				iPassword[pw_count++]='3';
				mark_star[star_count++]='*';
				delaynms(10);
				LCD_write_string(0,1,mark_star);
			}

			if(!(P2IN&BIT3)){
				delaynms(500);
				iPassword[pw_count++]='4';
				mark_star[star_count++]='*';
				delay_nms(10);
				LCD_write_string(0,1,mark_star);
			}

			if(!(P2IN&BIT4)){
				delaynms(500);
				Read_Password(c);
				Password_Validation(c);
				delay_nms(10);
				LCD_write_string(0,1,"                ");
			}
}


void main(void) {
	WDTCTL=WDTHOLD+WDTPW;
	P1DIR|=BIT3;
	P1DIR|=BIT0;
	P1OUT&=~BIT0;
	P1OUT&=~BIT3;
	LCD_init();
	Key_Init();
	LCD_write_string(0,0,"INFO:");
	while(1){
		Key_Press();
	}
}

