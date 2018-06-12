/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
#include "lcd1602.h"

#define CPU_F 1100000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delaynus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))


typedef unsigned char uchar;

#define RST BIT3
#define DAT BIT4
#define CLK BIT5

#define DS_RST_OUT P2DIR|=RST
#define DS_SCL_OUT P2DIR|=CLK
#define DS_SDA_OUT P2DIR|=DAT
#define DS_SDA_IN P2DIR&=~DAT

#define DS_RST0 P2OUT&=~RST
#define DS_RST1 P2OUT|=RST

#define DS_SCL0 P2OUT&=~CLK
#define DS_SCL1 P2OUT|=CLK

#define DS_SDA0 P2OUT&=~DAT
#define DS_SDA1 P2OUT|=DAT

#define DS_SDA_BIT (P2IN&DAT)

void DS1302_Init(void){
	DS_RST_OUT;
	DS_SCL_OUT;
	DS_SCL0;
	DS_RST0;
	__delay_cycles(10);
	DS_SCL1;
}

void WriteByte(uchar data){
	uchar i;
	DS_SDA_OUT;
	DS_RST1;
	for(i=8;i>0;i--){
		if(data&0x01)DS_SDA1;
		else DS_SDA0;
		DS_SCL1;
		DS_SCL0;
		data>>=1;
	}
}

uchar ReadByte(void){
	uchar i;
	uchar data=0x00;
	DS_SDA_IN;
	DS_RST1;
	for(i=8;i>0;i--){
		data>>=1;
		if(DS_SDA_BIT)data|=0x80;
		DS_SCL1;
		DS_SCL0;

	}
	return data;
}

void WriteData(uchar addr,uchar data){
	DS_RST0;
	DS_SCL0;
	DS_RST1;
	WriteByte(addr);
	WriteByte(data);
	DS_SCL1;
	DS_RST0;
}

uchar ReadData(uchar addr){
	uchar data;
	DS_RST0;
	DS_SCL0;
	DS_RST1;
	WriteByte(addr|0X01);
	data=ReadByte();
	DS_SCL1;
	DS_RST0;
	return data;
}

uchar time;
