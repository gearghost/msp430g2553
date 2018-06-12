/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
#include "lcd1602.h"

#define CPU_F 8000000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delaynus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))


#define DQ_OUT P2DIR|=BIT5
#define DQ_IN P2DIR&=~BIT5
#define DQ_1 P2OUT|=BIT5
#define DQ_0 P2OUT&=~BIT5
#define DQ_DATE (P2IN&BIT5)

#define READ_ROM 	0x33
#define MATCH_ROM 	0x55
#define SEARCH_ROM 	0xf0
#define ALERT_SEARCH 	0xec
#define COPY_SCRATCHPAD 	0x48
#define SKIP_ROM 	0xcc
#define READ_POWER 	0xb4

#define CONVERT_TEMPERATURE 	0x44
#define READ_SCRATCHPAD		 0xbe
#define WRITE_SCRATCHPAD 	0x4e
#define RECALL_E 	0xb8

typedef unsigned char uchar;
unsigned long int Temperature;
char Temp_str[16];


uchar DS18B20_Init(void){
	uchar result;
	DQ_OUT;
	DQ_0;
	delay_nus(500);
	DQ_IN;
	delay_nus(60);
	result=DQ_DATE;
	delay_nus(500);
	return result;
}

uchar DS18B20_ReadBit(void){
	uchar result;
	DQ_OUT;
	DQ_0;
	_NOP();
	DQ_IN;
	result=DQ_DATE;
	delay_nus(60);
	return result;

}

void DS18B20_WriteBit(uchar data){
	DQ_OUT;
	DQ_0;
	delay_nus(15);
	if(data)DQ_1;
	else DQ_0;
	delay_nus(60);
	DQ_IN;
}

uchar DS18B20_ReadByte(void){
	uchar i,result=0;
	for(i=0;i<8;i++){
		result>>=1;
		if(DS18B20_ReadBit())result|=0x80;
		else result&=0x7f;
	}
	return result;
}

void DS18B20_WriteByte(uchar data){
	unsigned char i;
	for(i=0;i<8;i++){
		DS18B20_WriteBit(data&0x01);
		data>>=1;
	}
}

void DS18B20_ReadTemp(void){
	uchar tempH,tempL;
	DS18B20_Init();
	DS18B20_WriteByte(SKIP_ROM);
	DS18B20_WriteByte(CONVERT_TEMPERATURE);
	delay_nms(1000);
	DS18B20_Init();
	DS18B20_WriteByte(SKIP_ROM);
	DS18B20_WriteByte(READ_SCRATCHPAD);
	tempL=DS18B20_ReadByte();
	tempH=DS18B20_ReadByte();
	Temperature=tempH;
	Temperature<<=8;
	Temperature+=tempL;
	Temperature=0.625*Temperature;
}

char *TemperatureToString(float d, char* str)
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
  for(i=0;i<10;i++)
  {
    d *= 10;
    str[j++] = (int)d+'0';
    d -= (int)d;
  }
  while(str[--j]=='0');
    str[++j] = '\0';
  return str;
}

int year=0;
int month=0;
int date=0;
int hour=0;
int minute=0;
int second=0;

char time[16];

void Clock_Init(int yr,int mon,int dt,int h,int m,int s){
	year=yr;
	month=mon;
	date=dt;
	hour=h;
	minute=m;
	second=s;
}

int Second(void){
	int i=0;
	while(i<4){
		delaynms(220);
		i++;
	}
	if(second==59){
		second=0;
		return second;
	}
	second++;
	return second;
}

int Minute(void){
	if(second==0){
		minute++;
		}
	if(minute==60){
			minute=0;
			return minute;
		}
	return minute;

}

int Hour(void){
	if(minute==0&&second==1)hour++;
	if(hour==24){
		hour=0;
		return hour;
	}
	return hour;
}



char *IntToString(int i,char *str){
	int a,b;
	a=i/10;
	b=(i-a*10);
	str[0]=a+0x30;
	str[1]=b+0x30;
	return str;
}



void main(void) {
	WDTCTL=WDTPW+WDTHOLD;
	BCSCTL1 = CALBC1_8MHZ;                    // Set DCO
	DCOCTL = CALDCO_8MHZ;
	LCD_init();
	float temp;
	int h,m,s;
	Clock_Init(14,11,20,0,0,0);
	while(1){
	   delaynms(1);
	   LCD_write_string(0,0,IntToString(month ,time));
	   delaynms(1);
	   LCD_write_string(2,0,"-");
	   delaynms(1);
	   LCD_write_string(3,0,IntToString(date,time));
	   delaynms(1);
	   h=Hour();
	   delaynms(1);
	   LCD_write_string(8,0,IntToString(h,time));
	   delaynms(1);
	   LCD_write_string(10,0,":");
	   delaynms(1);
	   m=Minute();
	   delaynms(1);
	   LCD_write_string(11,0,IntToString(m,time));
	   delaynms(1);
	   LCD_write_string(13,0,":");
	   delaynms(1);
	   s=Second();
	   delaynms(1);
	   LCD_write_string(14,0,IntToString(s,time));
	   delaynms(1);
	   DS18B20_ReadTemp();
	   temp=Temperature/10.0;
	   delaynms(1);
	   LCD_write_string(11,1,TemperatureToString(temp,Temp_str));
	   delaynms(1);
	   LCD_write_string(3,1,"Temptr:");
	   delaynms(1);
	}
}
