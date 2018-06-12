/*
 * main.c
 */

#include <msp430g2553.h>
#include <intrinsics.h>
#include "lcd1602.h"

#define CPU_F 1100000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delaynus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))

char r2l_str[32]="LaunchPad!";

void main(void) {
	WDTCTL=WDTPW+WDTHOLD;
	LCD_init();
	int i;
	int str_len;
	char *str_pt;
	while(1){
	delaynms(10);
	LCD_write_string(0,0,"                ");
	delaynms(10);
	for(i=16;i>=0;i--){
		delaynms(1);
					LCD_write_string(5,0,"Hello");
					delaynms(1);
		if((16-i)>=10)r2l_str[16-i]=' ';//少于16个字符时候的处理，现在不做处理
		delaynms(400);
		LCD_write_string(i,1,r2l_str);
		delaynms(400);
		}
	for(str_len=0;str_len<=10;str_len++){
		if(str_len==0)delaynms(10);
		else{
		delaynms(400);
		str_pt=&r2l_str[str_len];
		LCD_write_string(0,1,str_pt);
		delaynms(400);
	}
	}
}
}
