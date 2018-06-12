/*
 * lcd1602.h
 *
 *  Created on: Oct 23, 2014
 *      Author: Pal
 */

#ifndef LCD1602_H_
#define LCD1602_H_

//定义////////////////////////////////////////////////////////////////////
#define LCD_EN_PORT P1OUT//以下2个要设为同一个口
#define LCD_EN_DDR P1DIR
#define LCD_RS_PORT P1OUT//以下2个要设为同一个口
#define LCD_RS_DDR P1DIR
#define LCD_DATA_PORT P1OUT //以下3个要设为同一个口
#define LCD_DATA_DDR P1DIR //一定要用高4位
#define LCD_RS BIT1
#define LCD_EN BIT2
//#define LCD_DATA    BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7  //8位数据线连接模式时使用
#define LCD_DATA    BIT4|BIT5|BIT6|BIT7   //4位数据线连接模式时使用
////预定义函数//////////////////////////////////////////////////////////////////
void LCD_init(void);
void LCD_en_write(void);
void LCD_write_command(unsigned char command);
void LCD_write_data(unsigned char data);
void LCD_set_xy (unsigned char x, unsigned char y);
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s);
void LCD_write_char(unsigned char X,unsigned char Y,unsigned char data);
void delay_1ms(void);
void delay_nus(unsigned int n);
void delay_nms(unsigned int n);
//LCD液晶操作函数///////////////////////////////////////////////////////////////
//LCD1602液晶初始化
void LCD_init(void)
{
	delay_nms(15);
	LCD_DATA_DDR|=LCD_DATA; //数据口方向为输出
	LCD_EN_DDR|=LCD_EN; //设置EN方向为输出
	LCD_RS_DDR|=LCD_RS; //设置RS方向为输出

	delay_1ms();		      //这4行不要改，否则仿真没显示
//	LCD_write_command(0x38);  //8位数据接口时，使用这行，否则仿真没显示
	LCD_write_command(0x33);  //4位数据接口时，使用这行，否则仿真没显示
	delay_1ms();		     //
//	LCD_write_command(0x38);  //8位数据接口时，使用这行，否则仿真没显示
	LCD_write_command(0x32);  //4位数据接口时，使用这行，否则仿真没显示
	delay_1ms();
//	LCD_write_command(0x38); //8位数据接口时，使用这行，否则仿真没显示


	delay_1ms();
//	LCD_write_command(0x38); //8位数据接口
	LCD_write_command(0x2c); //4位数据接口

	delay_1ms();
	LCD_write_command(0x0c); //显示开

	delay_1ms();
	LCD_write_command(0x01); //清屏

	delay_1ms();
	LCD_write_command(0x06);

}
//液晶使能
void LCD_en_write(void)
{
LCD_EN_PORT|=LCD_EN;
delay_nus(10);
LCD_EN_PORT&=~LCD_EN;
}
//写指令
void LCD_write_command(unsigned char command)
{
delay_nus(16);
LCD_RS_PORT&=~LCD_RS; //RS=0
LCD_DATA_PORT&=0X0f; //清高四位
LCD_DATA_PORT|=command&0xf0; //写高四位
//LCD_DATA_PORT = command; //写8位

delay_nus(16);
LCD_en_write();
command=command<<4; //低四位移到高四位
LCD_DATA_PORT&=0x0f; //清高四位
LCD_DATA_PORT|=command&0xf0; //写低四位

LCD_en_write();
}
//写数据
void LCD_write_data(unsigned char data)
{
delay_nus(16);
LCD_RS_PORT|=LCD_RS; //RS=1
LCD_DATA_PORT&=0X0f; //清高四位
LCD_DATA_PORT|=data&0xf0; //写高四位
//LCD_DATA_PORT = data; //写8位

LCD_en_write();
data=data<<4; //低四位移到高四位
LCD_DATA_PORT&=0X0f; //清高四位
LCD_DATA_PORT|=data&0xf0; //写低四位
LCD_en_write();
}
//写地址函数
void LCD_set_xy( unsigned char x, unsigned char y )
{
unsigned char address;
if (y == 0) address = 0x80 + x;
else address = 0xc0 + x;
LCD_write_command( address);
}
//LCD在任意位置写字符串
//列x=0~15,行y=0,1
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s)
{
	LCD_set_xy( X, Y ); //写地址
while (*s) // 写显示字符
{
LCD_write_data( *s );
s ++;
}
}
//LCD在任意位置写字符
//列x=0~15,行y=0,1
void LCD_write_char(unsigned char X,unsigned char Y,unsigned char data)
{
LCD_set_xy( X, Y ); //写地址
LCD_write_data( data);
}
//延时函数//////////////////////////////////////////////////////////////////////
//1us延时函数
void delay_1us(void)
{
asm("nop");
}
//N us延时函数
void delay_nus(unsigned int n)
{
unsigned int i;
for (i=0;i<n;i++)
delay_1us();
}
//1ms延时函数
void delay_1ms(void)
{
unsigned int i;
for (i=0;i<1140;i++);
}
//N ms延时函数
void delay_nms(unsigned int n)
{
unsigned int i=0;
for (i=0;i<n;i++)
delay_1ms();
}



#endif /* LCD1602_H_ */
