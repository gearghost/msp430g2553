/*
 * main.c
 */
#include <msp430f2272.h>
#include <intrinsics.h>
#include <math.h>
#include "lcd1602.h"

#define CPU_F 8000000
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delaynus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))

typedef unsigned char uchar;
typedef unsigned int uint;

/*数字字符队列*/

typedef struct Number{
	uchar num[5];
	int head;
	int tail;
}Number;

uchar first_line[16];
uint c_index=0;
uint flag=0;
int sym_pos[2];
int sym_count=0;
uchar result_str[16];

Number first_para;
Number second_para;
long para[2];

int carry_table[4]={1,10,100,1000};


/*字符串转整型*/
long StringToLong(Number num_str){
	long num=0;
	int table_index;
	table_index=num_str.tail-1;
	while(num_str.head!=num_str.tail){
		num+=(int)(num_str.num[num_str.head++]-0x30)*carry_table[table_index];
		table_index--;
	}
	return num;
}


/*长整型转字符串*/
uchar *LongToString(long num,uchar *str){
	int i=0;
	long  tenmillion,million,hundredthousand,tenthousand,thousand,hundred,ten,unit;
	tenmillion=num/10000000;
	million=(num-tenmillion*10000000)/1000000;
	hundredthousand=(num-tenmillion*10000000-million*1000000)/100000;
	tenthousand=(num-tenmillion*10000000-million*1000000-hundredthousand*100000)/10000;
	thousand=(num-tenmillion*10000000-million*1000000-hundredthousand*100000-tenthousand*10000)/1000;
	hundred=(num-tenmillion*10000000-million*1000000-hundredthousand*100000-tenthousand*10000-thousand*1000)/100;
	ten=(num-tenmillion*10000000-million*1000000-hundredthousand*100000-tenthousand*10000-thousand*1000-hundred*100)/10;
	unit=num-tenmillion*10000000-million*1000000-hundredthousand*100000-tenthousand*10000-thousand*1000-hundred*100-ten*10;
	if(tenmillion)str[i++]=tenmillion+0x30;
	if(tenmillion||million)str[i++]=million+0x30;
	if(tenmillion||million||hundredthousand)str[i++]=hundredthousand+0x30;
	if(tenmillion||million||hundredthousand||tenthousand)str[i++]=tenthousand+0x30;
	if(tenmillion||million||hundredthousand||tenthousand||thousand)str[i++]=thousand+0x30;
	if(tenmillion||million||hundredthousand||tenthousand||thousand||hundred)str[i++]=hundred+0x30;
	if(tenmillion||million||hundredthousand||tenthousand||thousand||hundred||ten)str[i++]=ten+0x30;
	str[i]=unit+0x30;
	return str;
}


/*公式解析*/
long *Formula_Parse(uchar *formula,int *symbol_pos,long *parse_num){
	first_para.head=0;
	int i,j;
	for(i=0;i<symbol_pos[0];i++){
		first_para.num[i]=formula[i];
	}
	first_para.tail=i;
	second_para.head=0;
	for(i=symbol_pos[0]+1,j=0;i<symbol_pos[1];i++){
		second_para.num[j++]=formula[i];
	}
	second_para.tail=j;
	parse_num[0]=StringToLong(first_para);
	parse_num[1]=StringToLong(second_para);
	return parse_num;
}

long Calc(uchar *str,int *symb_pos){
	long result=0;
	double d_result=0.0;
	Formula_Parse(str,symb_pos,para);
	uchar symbol;
	symbol=str[symb_pos[0]];
	switch(symbol){
		case '+':
				{
					result=para[0]+para[1];
					break;
				}
		case '-':
				{
					result=para[0]-para[1];
					break;
				}
		case '*':
				{
					result=para[0]*para[1];
					break;
				}
		case '/':
				{
					d_result=(double)para[0]/(double)para[1] ;
					break;
				}
		case '^':
				{
					result=para[0]*para[0];
					break;
				}
		case 251:
				{
					d_result=sqrt(para[1]);
					break;
				}
		default:
					break;
	}
	  if(symbol=='/'||symbol==251)return (long)d_result;
	  else return result;
}

void Reset(void){
	int i;
	for(i=0;i<16;i++)first_line[i]=' ';
	c_index=0;
	for(i=0;i<16;i++)result_str[i]=' ';
    sym_count=0;
}



/*初始化按键*/
void Key_Init(void){

	P1DIR|=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;
	P4DIR|=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;
	P1REN|=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;
	P4REN|=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;
	P1OUT|=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;
	P4OUT|=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;
	P1IES|=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;
	P1IFG&=~BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;
	P1IE|=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;
}

/*按键轮询*/
void Key_Press(void){
	long result;
	if(!(P1IN&BIT0)){
				delaynms(500);
				sym_pos[sym_count++]=c_index;
				if(flag){
					first_line[c_index++]=251;//根号的ASCII码，不明觉厉
					flag=0;
				}
				else first_line[c_index++]='+';
				delaynms(10);
				LCD1602_Write_String(0,0,first_line);
				delaynms(10);
			}
			if(!(P1IN&BIT1)){
						delaynms(500);
						sym_pos[sym_count++]=c_index;
						if(flag){
									first_line[c_index++]=94;//平方符号
									first_line[c_index++]='2';
									flag=0;

						}
						else first_line[c_index++]='-';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P1IN&BIT2)){
						delaynms(500);
						sym_pos[sym_count++]=c_index;
						if(flag)
							{
							first_line[c_index++]='=';
							flag=0;
							}
						else first_line[c_index++]='*';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P1IN&BIT3)){
						delaynms(500);
						sym_pos[sym_count++]=c_index;
						if(flag)
							{
								Reset();
								flag=0;
								delaynms(10);
								LCD1602_Write_String(0,1,result_str);
								delaynms(10);
							}
						else first_line[c_index++]='/';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P1IN&BIT4)){
						delaynms(500);
						sym_pos[sym_count++]=c_index;
						first_line[c_index++]='=';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						result=Calc(first_line,sym_pos);
						delaynms(10);
						LCD1602_Write_String(0,1,"ANS:");
						delaynms(10);
						LCD1602_Write_String(4,1,LongToString(result,result_str));
						delaynms(10);
					}
			if(!(P1IN&BIT5)){
						delaynms(500);
						first_line[c_index++]='0';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P1IN&BIT6)){
						delaynms(500);
						flag=1;
						delaynms(10);
					}
			if(!(P1IN&BIT7)){
						delaynms(500);
						first_line[c_index++]='9';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P4IN&BIT0)){
						delaynms(500);
						first_line[c_index++]='1';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P4IN&BIT1)){
						delaynms(500);
						first_line[c_index++]='2';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P4IN&BIT2)){
						delaynms(500);
						first_line[c_index++]='3';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P4IN&BIT3)){
						delaynms(500);
						first_line[c_index++]='4';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P4IN&BIT4)){
						delaynms(500);
						first_line[c_index++]='5';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P4IN&BIT5)){
						delaynms(500);
						first_line[c_index++]='6';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P4IN&BIT6)){
						delaynms(500);
						first_line[c_index++]='7';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}
			if(!(P4IN&BIT7)){
					    delaynms(500);
						first_line[c_index++]='8';
						delaynms(10);
						LCD1602_Write_String(0,0,first_line);
						delaynms(10);
					}

}

uchar flowing_str[24]="Welcome!";

/*开机流动字符*/
void ScreenOn(void){
	int i;
	int str_len;
	uchar *str_pt;
	for(i=16;i>=0;i--){
		if((16-i)>=8)flowing_str[16-i]=' ';//少于16个字符时候的处理，现在不做处理
		delaynms(100);
		LCD1602_Write_String(i,0,flowing_str);
		delaynms(100);
		}
	for(str_len=0;str_len<=8;str_len++){
		if(str_len==0)delaynms(10);
		else delaynms(100);
		str_pt=&flowing_str[str_len];
		LCD1602_Write_String(0,0,str_pt);
		delaynms(100);
	}
}

void main(void){
	WDTCTL=WDTPW+WDTHOLD;
	BCSCTL1 = CALBC1_8MHZ;                    // Set DCO
	DCOCTL = CALDCO_8MHZ;
	Key_Init();
	LCD1602_Init();
	ScreenOn();
	LCD1602_Write_String(0,0,"0");
	while(1){
		Key_Press();
	}
}
