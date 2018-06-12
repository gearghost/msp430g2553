#ifndef _LQOLED_H
#define _LQOLED_H
#include <msp430g2553.h>


#define byte char
#define word unsigned short int
#define uchar char
#define u8 char
#define u32 unsigned long int
#define u16 unsigned short int


#define CS_1 P1OUT|=BIT7
#define CS_0 P1OUT&=~BIT7

#define DC_1 P1OUT|=BIT6
#define DC_0 P1OUT&=~BIT6

#define MOSI_1 P1OUT|=BIT5
#define MOSI_0 P1OUT&=~BIT5

#define CLK_1 P1OUT|=BIT4
#define CLK_0 P1OUT&=~BIT4

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF

/*
4线SPI使用说明：
VBT 供内部DC-DC电压，3.3~4.3V，如果使用5V电压，为保险起见串一个100~500欧的电阻
VCC 供内部逻辑电压 1.8~6V
GND 地

BS0 低电平
BS1 低电平
BS2 低电平

CS  片选管脚
DC  命令数据选择管脚
RES 模块复位管脚
D0（SCLK） ，时钟脚，由MCU控制
D1（MOSI） ，主输出从输入数据脚，由MCU控制

D2 悬空
D3-D7 ， 低电平 ， 也可悬空，但最好设为低电平
RD  低电平 ，也可悬空，但最好设为低电平
RW  低电平 ，也可悬空，但最好设为低电平
RD  低电平 ，也可悬空，但最好设为低电平
*/

#define X_WIDTH 128
#define Y_WIDTH 64
//======================================

uchar num[5];
uchar ad_num[3];
uchar num32[10];

const byte F6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(byte x,byte y,byte ch[]);
 void LCD_PutPixel(byte x,byte y);
 void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
 void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]); 
 void LCD_Fill(byte dat);
 uchar *Int32ToStr(u32 num,uchar *str);
 void NumArray_Clr(uchar *str,u16 len);
 void Display_Items(void);
 void Display_ServoData(u16 pwm);
 void Display_MotoData(u16 pwm);
 void Display_EncoderData(u16 pulse);
 void Display_PD(u8 p,u8 d);
 void Display_PID(u8 p,u8 i,u8 d);
 void Display_M1_M2(u8 m1,u8 m2);
 void Display_ll(u8 lflag);
 void Display_AD(u8 adc0,u8 adc1,u8 adc2,u8 adc3,u8 adc4,u8 adc5);
 void Display_NRF24L01(u32 data);
 void Display_Flag(u8 dflag);

 void LCD_WrDat(byte data)
 {
 	byte i=8;
 	CS_0;;
 	DC_1;;
         //asm("NOP");
         CLK_0;;
       //asm("NOP");
   while(i--)
   {
     if(data&0x80){MOSI_1;}
     else{MOSI_0;}
     CLK_1;
     //asm("NOP");
 		//asm("nop");
     CLK_0;;
     data<<=1;
   }
 	CS_1;
 }
 void LCD_WrCmd(byte cmd)
 {
 	byte i=8;

 	CS_0;;
   DC_0;;
   CLK_0;;
   //asm("nop");
   while(i--)
   {
     if(cmd&0x80){MOSI_1;}
     else{MOSI_0;}
     CLK_1;
     //asm("NOP");
 		//asm("nop");
     CLK_0;;
     cmd<<=1;
   }
 	CS_1;
 }
 void LCD_Set_Pos(byte x, byte y)
 {
   LCD_WrCmd(0xb0+y);
   LCD_WrCmd(((x&0xf0)>>4)|0x10);
   LCD_WrCmd((x&0x0f)|0x01);
 }
 void LCD_Fill(byte bmp_data)
 {
 	byte y,x;

 	for(y=0;y<8;y++)
 	{
 		LCD_WrCmd(0xb0+y);
 		LCD_WrCmd(0x01);
 		LCD_WrCmd(0x10);
 		for(x=0;x<X_WIDTH;x++)
 			LCD_WrDat(bmp_data);
 	}
 }
 void LCD_CLS(void)
 {
 	byte y,x;
 	for(y=0;y<8;y++)
 	{
 		LCD_WrCmd(0xb0+y);
 		LCD_WrCmd(0x01);
 		LCD_WrCmd(0x10);
 		for(x=0;x<X_WIDTH;x++)
 			LCD_WrDat(0);
 	}
 }
 void LCD_DLY_ms(word ms)
 {
   word a;
   while(ms)
   {
     a=13350;
     while(a--);
     ms--;
   }
   return;
 }

 void LCD_Init(void)
 {
         //设置PORTA pin14,pin15为GPIO口
 	/*PORTA_PCR14=(0|PORT_PCR_MUX(1));
 	PORTA_PCR15=(0|PORT_PCR_MUX(1));
 	PORTA_PCR16=(0|PORT_PCR_MUX(1));
 	PORTA_PCR17=(0|PORT_PCR_MUX(1));

 	//设置PORTA pin14,pin15为输出方向;pin16,pin17为输入方向
 	GPIOA_PDDR=GPIO_PDDR_PDD(GPIO_PIN(14)|GPIO_PIN(15)|GPIO_PIN(16)|GPIO_PIN(17));
 	*/

 	CLK_1;
 	CS_1;	//预制SLK和SS为高电平

 	//RST_0;
 	LCD_DLY_ms(50);
 	//RST_1;

   LCD_WrCmd(0xae);//--turn off oled panel
   LCD_WrCmd(0x00);//---set low column address
   LCD_WrCmd(0x10);//---set high column address
   LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
   LCD_WrCmd(0x81);//--set contrast control register
   LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
   LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
   LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
   LCD_WrCmd(0xa6);//--set normal display
   LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
   LCD_WrCmd(0x3f);//--1/64 duty
   LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
   LCD_WrCmd(0x00);//-not offset
   LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
   LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
   LCD_WrCmd(0xd9);//--set pre-charge period
   LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
   LCD_WrCmd(0xda);//--set com pins hardware configuration
   LCD_WrCmd(0x12);
   LCD_WrCmd(0xdb);//--set vcomh
   LCD_WrCmd(0x40);//Set VCOM Deselect Level
   LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
   LCD_WrCmd(0x02);//
   LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
   LCD_WrCmd(0x14);//--set(0x10) disable
   LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
   LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
   LCD_WrCmd(0xaf);//--turn on oled panel
   LCD_Fill(0x00);  //初始清屏
   LCD_Set_Pos(0,0);

 }
 //==============================================================
 //函数名： void LCD_PutPixel(byte x,byte y)
 //功能描述：绘制一个点（x,y）
 //参数：真实坐标值(x,y),x的范围0～127，y的范围0～64
 //返回：无
 //==============================================================
 void LCD_PutPixel(byte x,byte y)
 {
 	byte data1;  //data1当前点的数据

   LCD_Set_Pos(x,y);
 	data1 = 0x01<<(y%8);
 	LCD_WrCmd(0xb0+(y>>3));
 	LCD_WrCmd(((x&0xf0)>>4)|0x10);
 	LCD_WrCmd((x&0x0f)|0x00);
 	LCD_WrDat(data1);
 }
 //==============================================================
 //函数名： void LCD_Rectangle(byte x1,byte y1,
 //                   byte x2,byte y2,byte color,byte gif)
 //功能描述：绘制一个实心矩形
 //参数：左上角坐标（x1,y1）,右下角坐标（x2，y2）
 //      其中x1、x2的范围0～127，y1，y2的范围0～63，即真实坐标值
 //返回：无
 //==============================================================
 void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif)
 {
 	byte n;

 	LCD_Set_Pos(x1,y1>>3);
 	for(n=x1;n<=x2;n++)
 	{
 		LCD_WrDat(0x01<<(y1%8));
 		if(gif == 1) 	LCD_DLY_ms(50);
 	}
 	LCD_Set_Pos(x1,y2>>3);
   for(n=x1;n<=x2;n++)
 	{
 		LCD_WrDat(0x01<<(y2%8));
 		if(gif == 1) 	LCD_DLY_ms(5);
 	}

 }
 //==============================================================
 //函数名：LCD_P6x8Str(byte x,byte y,byte *p)
 //功能描述：写入一组标准ASCII字符串
 //参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
 //返回：无
 //==============================================================
 void LCD_P6x8Str(byte x,byte y,byte ch[])
 {
   byte c=0,i=0,j=0;
   while (ch[j]!='\0')
   {
     c =ch[j]-32;
     if(x>126){x=0;y++;}
     LCD_Set_Pos(x,y);
   	for(i=0;i<6;i++)
   	  LCD_WrDat(F6x8[c][i]);
   	x+=6;
   	j++;
   }
 }
 //==============================================================
 //函数名： void Draw_BMP(byte x,byte y)
 //功能描述：显示BMP图片128×64
 //参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7
 //返回：无
 //==============================================================
 void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[])
 {
   word ii=0;
   byte x,y;

   if(y1%8==0) y=y1/8;
   else y=y1/8+1;
 	for(y=y0;y<=y1;y++)
 	{
 		LCD_Set_Pos(x0,y);
     for(x=x0;x<x1;x++)
 	    {
 	    	LCD_WrDat(bmp[ii++]);
 	    }
 	}
 }

 /* Function for displaying data */

 uchar *Int32ToStr(u32 num,uchar *str){
   int mod;
   int i=0,j=0,mid;
   char temp;
   while(num/10!=0){
     mod=num%10;
     str[i++]=mod+0x30;
     num=num/10;
   }
   str[i]=num+0x30;
   mid=i/2;
   while(j<=mid){
     temp=str[i];
     str[i]=str[j];
     str[j]=temp;
     i--;
     j++;
   }
   return str;
 }

 void NumArray_Clr(uchar *str,u16 len){
   u16 i;
   for(i=0;i<len;i++)str[i]=' ';
 }

 void Display_Items(void){
   LCD_P6x8Str(0,0,"Sensor:");
   LCD_P6x8Str(0,2,"Parameter:");
   LCD_P6x8Str(0,3,"S:");
   LCD_P6x8Str(48,3,"P:");
   LCD_P6x8Str(84,3,"D:");
   LCD_P6x8Str(0,4,"E:");
   LCD_P6x8Str(48,4,"R:");
   LCD_P6x8Str(0,5,"P:");
   LCD_P6x8Str(36,5,"I:");
   LCD_P6x8Str(72,5,"D:");
   LCD_P6x8Str(0,6,"GH:");
 }

 void Display_ServoData(u16 pwm){
   LCD_P6x8Str(12,3,Int32ToStr(pwm,num));
   NumArray_Clr(num,5);
 }

 void Display_MotoData(u16 pwm){
   LCD_P6x8Str(60,4,Int32ToStr(pwm,num));
   NumArray_Clr(num,5);
 }

 void Display_EncoderData(u16 pulse){
   LCD_P6x8Str(12,4,Int32ToStr(pulse,num));
   NumArray_Clr(num,5);
 }

 void Display_PD(u8 p,u8 d){
   LCD_P6x8Str(60,3,Int32ToStr(p,ad_num));
   NumArray_Clr(ad_num,3);
   LCD_P6x8Str(96,3,Int32ToStr(d,ad_num));
   NumArray_Clr(ad_num,3);
 }

 void Display_PID(u8 p,u8 i,u8 d){
   LCD_P6x8Str(12,5,Int32ToStr(p,ad_num));
   NumArray_Clr(ad_num,3);
   LCD_P6x8Str(48,5,Int32ToStr(i,ad_num));
   NumArray_Clr(ad_num,3);
   LCD_P6x8Str(84,5,Int32ToStr(d,ad_num));
   NumArray_Clr(ad_num,3);
 }

 void Display_M1_M2(u8 m1,u8 m2){
   LCD_P6x8Str(64,2,Int32ToStr(m1,ad_num));
   NumArray_Clr(ad_num,3);
   LCD_P6x8Str(105,2,Int32ToStr(m2,ad_num));
   NumArray_Clr(ad_num,3);
 }

 void Display_ll(u8 lflag){
   if(lflag==0)LCD_P6x8Str(121,4,"0");
   else if(lflag==1)LCD_P6x8Str(121,4,"1");
   else LCD_P6x8Str(121,4,"!");
 }

 void Display_AD(u8 adc0,u8 adc1,u8 adc2,u8 adc3,u8 adc4,u8 adc5){
   LCD_P6x8Str(0,1,Int32ToStr(adc0,ad_num));
   NumArray_Clr(ad_num,3);
   LCD_P6x8Str(21,1,Int32ToStr(adc1,ad_num));
   NumArray_Clr(ad_num,3);
   LCD_P6x8Str(43,1,Int32ToStr(adc2,ad_num));
   NumArray_Clr(ad_num,3);
   LCD_P6x8Str(65,1,Int32ToStr(adc3,ad_num));
   NumArray_Clr(ad_num,3);
   LCD_P6x8Str(87,1,Int32ToStr(adc4,ad_num));
   NumArray_Clr(ad_num,3);
   LCD_P6x8Str(109,1,Int32ToStr(adc5,ad_num));
   NumArray_Clr(ad_num,3);
 }

 void Display_NRF24L01(u32 data){
   LCD_P6x8Str(0,7,Int32ToStr(data,num32));
   NumArray_Clr(num,10);
 }

 void Display_Flag(u8 dflag){
   if(dflag==0)LCD_P6x8Str(50,6,"0");
   else if(dflag==1)LCD_P6x8Str(50,6,"1");
   else LCD_P6x8Str(50,6,Int32ToStr(dflag,ad_num));
   NumArray_Clr(ad_num,3);
 }

#endif

