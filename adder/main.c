

/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
#include "lcd1602.h"

#define CPU_F 1100000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))

int count=0;
char formula[16];

void Calc(char *str){
	int first,last,result;
	if(str[0]=='~'){
		switch(str[1]){
			case '1':
					str[count++]='0';
					break;
			case '0':
					str[count++]='1';
					break;
			default:
					break;
	}
	}
	else{
	switch(str[0]){
	case '1':
			first=1;
			break;
	case '0':
			first=0;
			break;
	default:
			break;
	}

	switch(str[2]){
	case '1':
			last=1;
			break;
	case '0':
			last=0;
			break;
	default:
			break;
	}

	switch(str[1]){
	   case '&':
		   result=first&last;
		   break;
	   case '|':
		   result=first|last;
		   break;
	   default:
		   break;

	}

	switch(result){
	  case 1:
		  str[count++]='1';
		  break;
	  case 0:
		  str[count++]='0';
		  break;
	  default:
		  break;
	}
	}
	return;
}

void main(void) {
	WDTCTL=WDTPW+WDTHOLD;
    P2SEL&=~BIT7;
	P2REN|=BIT5+BIT4+BIT3+BIT0+BIT1+BIT2+BIT7;
	P2OUT|=BIT5+BIT4+BIT3+BIT0+BIT1+BIT2+BIT7;
	LCD_init();
	delay_1ms();
	LCD_write_string(0,0,"LOGIC CACULATOR");
	delay_nms(10);
	while(1)
	{
		if(!(P2IN&BIT5)){
			delaynms(500);
			formula[count++]='|';
			delaynms(10);
			LCD_write_string(0,1,formula);
			delaynms(10);
		}

		if(!(P2IN&BIT4)){
			delaynms(500);
			formula[count++]='&';
			delaynms(10);
			LCD_write_string(0,1,formula);
			delaynms(10);
		}

		if(!(P2IN&BIT3)){
			delaynms(500);
			formula[count++]='1';
			delaynms(10);
			LCD_write_string(0,1,formula);
			delaynms(10);
		}

		if(!(P2IN&BIT0)){
			delaynms(500);
			formula[count++]='0';
			delaynms(10);
			LCD_write_string(0,1,formula);
			delaynms(10);
		}

		if(!(P2IN&BIT1)){
			delaynms(500);
			formula[count++]='~';
			delaynms(10);
			LCD_write_string(0,1,formula);
			delaynms(10);
		}

		if(!(P2IN&BIT2)){
			delaynms(500);
			formula[count-1]=' ';
			delaynms(10);
			LCD_write_string(0,1,formula);
			delaynms(10);
			count--;
		}

		if(!(P2IN&BIT7)){
			delaynms(500);
			formula[count++]='=';
			Calc(formula);
			delaynms(10);
			LCD_write_string(0,1,formula);
			delaynms(10);
		}
	}
}
