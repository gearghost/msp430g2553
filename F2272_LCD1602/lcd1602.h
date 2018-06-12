/*
 * lcd1602.h
 *
 *  Created on: Nov 28, 2014
 *      Author: Pal
 */

#ifndef LCD1602_H_
#define LCD1602_H_


#define RS_DIR P2DIR
#define RS_OUT P2OUT
#define RS BIT0
#define RS_1 P2OUT|=BIT0
#define RS_0 P2OUT&=~BIT0


#define RW_DIR P2DIR
#define RW_OUT P2OUT
#define RW BIT1
#define RW_1 P2OUT|=BIT1
#define RW_0 P2OUT&=~BIT1

#define EN_DIR P2DIR
#define EN_OUT P2OUT
#define EN BIT2
#define EN_1 P2OUT|=BIT2
#define EN_0 P2OUT&=~BIT2

#define DATA_DIR P3DIR
#define DATA_OUT P3OUT
#define LCD_DATA_PORT BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7

void LCD1602_Init(void);
void LCD1602_Write_EN(void);
void LCD1602_Write_Command(unsigned char cmd);
void LCD1602_Write_Data(unsigned char data);
void LCD1602_Set_XY(unsigned char x,unsigned char y);
void LCD1602_Write_String(unsigned char x,unsigned char y,unsigned char *str);
void LCD1602_Write_Char(unsigned char x,unsigned char y,unsigned char ch);
void delay_1us(void);
void delay_1ms(void);
void delay_nus(unsigned int n);
void delay_nms(unsigned int n);

void LCD1602_Init(void){
	delay_nms(15);
	RS_DIR|=RS;
	RW_DIR|=RW;
	EN_DIR|=EN;
	DATA_DIR|=LCD_DATA_PORT;
	RW_0;
	delay_1ms();
	LCD1602_Write_Command(0x38);
	delay_1ms();
	LCD1602_Write_Command(0x38);
	delay_1ms();
	LCD1602_Write_Command(0x38);
	delay_1ms();
	LCD1602_Write_Command(0x38);
	delay_1ms();
	LCD1602_Write_Command(0x0c);
	delay_1ms();
	LCD1602_Write_Command(0x01);
	delay_1ms();
	LCD1602_Write_Command(0x06);

}

void LCD1602_Write_EN(void){
	EN_1;
	delay_nus(10);
	EN_0;
}

void LCD1602_Write_Command(unsigned char cmd){
	delay_nus(16);
	RS_0;
	DATA_OUT=cmd;
	delay_nus(16);
	LCD1602_Write_EN();
}

void LCD1602_Write_Data(unsigned char data){
	delay_nus(16);
	RS_1;
	DATA_OUT=data;
	LCD1602_Write_EN();
}

void LCD1602_Set_XY(unsigned char x,unsigned char y){
	unsigned char addr;
	if(y==1)addr=0xc0+x;
	else addr=0x80+x;
	LCD1602_Write_Command(addr);
}

void LCD1602_Write_String(unsigned char x,unsigned char y,unsigned char *str){
	LCD1602_Set_XY(x,y);
	while(*str){
		LCD1602_Write_Data(*str);
		str++;
	}
}

void LCD1602_Write_Char(unsigned char x,unsigned char y,unsigned char ch){
	LCD1602_Set_XY(x,y);
	LCD1602_Write_Data(ch);
}

void delay_1us(void){
	asm("nop");
}

void delay_1ms(void){
	int i;
	for(i=0;i<1140;i++){
		delay_1us();
	}
}

void delay_nus(unsigned int n){
	int i;
	for(i=0;i<n;i++){
		delay_1us();
	}
}

void delay_nms(unsigned int n){
	int i;
	for(i=0;i<n;i++){
		delay_1ms();
	}
}


#endif /* LCD1602_H_ */
