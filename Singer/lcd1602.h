/*
 * lcd1602.h
 *
 *  Created on: Oct 23, 2014
 *      Author: Pal
 */

#ifndef LCD1602_H_
#define LCD1602_H_

//å®šä¹‰////////////////////////////////////////////////////////////////////
#define LCD_EN_PORT P2OUT//ä»¥ä¸‹2ä¸ªè¦�è®¾ä¸ºå�Œä¸€ä¸ªå�£
#define LCD_EN_DDR P2DIR
#define LCD_RS_PORT P1OUT//ä»¥ä¸‹2ä¸ªè¦�è®¾ä¸ºå�Œä¸€ä¸ªå�£
#define LCD_RS_DDR P1DIR
#define LCD_DATA_PORT P1OUT //ä»¥ä¸‹3ä¸ªè¦�è®¾ä¸ºå�Œä¸€ä¸ªå�£
#define LCD_DATA_DDR P1DIR //ä¸€å®šè¦�ç”¨é«˜4ä½�
#define LCD_RS BIT3
#define LCD_EN BIT0
//#define LCD_DATA    BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7  //8ä½�æ•°æ�®çº¿è¿žæŽ¥æ¨¡å¼�æ—¶ä½¿ç”¨
#define LCD_DATA    BIT4|BIT5|BIT6|BIT7   //4ä½�æ•°æ�®çº¿è¿žæŽ¥æ¨¡å¼�æ—¶ä½¿ç”¨
////é¢„å®šä¹‰å‡½æ•°//////////////////////////////////////////////////////////////////
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
//LCDæ¶²æ™¶æ“�ä½œå‡½æ•°///////////////////////////////////////////////////////////////
//LCD1602æ¶²æ™¶åˆ�å§‹åŒ–
void LCD_init(void)
{
	delay_nms(15);
	LCD_DATA_DDR|=LCD_DATA; //æ•°æ�®å�£æ–¹å�‘ä¸ºè¾“å‡º
	LCD_EN_DDR|=LCD_EN; //è®¾ç½®ENæ–¹å�‘ä¸ºè¾“å‡º
	LCD_RS_DDR|=LCD_RS; //è®¾ç½®RSæ–¹å�‘ä¸ºè¾“å‡º

	delay_1ms();		      //è¿™4è¡Œä¸�è¦�æ”¹ï¼Œå�¦åˆ™ä»¿çœŸæ²¡æ˜¾ç¤º
//	LCD_write_command(0x38);  //8ä½�æ•°æ�®æŽ¥å�£æ—¶ï¼Œä½¿ç”¨è¿™è¡Œï¼Œå�¦åˆ™ä»¿çœŸæ²¡æ˜¾ç¤º
	LCD_write_command(0x33);  //4ä½�æ•°æ�®æŽ¥å�£æ—¶ï¼Œä½¿ç”¨è¿™è¡Œï¼Œå�¦åˆ™ä»¿çœŸæ²¡æ˜¾ç¤º
	delay_1ms();		     //
//	LCD_write_command(0x38);  //8ä½�æ•°æ�®æŽ¥å�£æ—¶ï¼Œä½¿ç”¨è¿™è¡Œï¼Œå�¦åˆ™ä»¿çœŸæ²¡æ˜¾ç¤º
	LCD_write_command(0x32);  //4ä½�æ•°æ�®æŽ¥å�£æ—¶ï¼Œä½¿ç”¨è¿™è¡Œï¼Œå�¦åˆ™ä»¿çœŸæ²¡æ˜¾ç¤º
	delay_1ms();
//	LCD_write_command(0x38); //8ä½�æ•°æ�®æŽ¥å�£æ—¶ï¼Œä½¿ç”¨è¿™è¡Œï¼Œå�¦åˆ™ä»¿çœŸæ²¡æ˜¾ç¤º


	delay_1ms();
//	LCD_write_command(0x38); //8ä½�æ•°æ�®æŽ¥å�£
	LCD_write_command(0x2c); //4ä½�æ•°æ�®æŽ¥å�£

	delay_1ms();
	LCD_write_command(0x0c); //æ˜¾ç¤ºå¼€

	delay_1ms();
	LCD_write_command(0x01); //æ¸…å±�

	delay_1ms();
	LCD_write_command(0x06);

}
//æ¶²æ™¶ä½¿èƒ½
void LCD_en_write(void)
{
LCD_EN_PORT|=LCD_EN;
delay_nus(10);
LCD_EN_PORT&=~LCD_EN;
}
//å†™æŒ‡ä»¤
void LCD_write_command(unsigned char command)
{
delay_nus(16);
LCD_RS_PORT&=~LCD_RS; //RS=0
LCD_DATA_PORT&=0X0f; //æ¸…é«˜å››ä½�
LCD_DATA_PORT|=command&0xf0; //å†™é«˜å››ä½�
//LCD_DATA_PORT = command; //å†™8ä½�

delay_nus(16);
LCD_en_write();
command=command<<4; //ä½Žå››ä½�ç§»åˆ°é«˜å››ä½�
LCD_DATA_PORT&=0x0f; //æ¸…é«˜å››ä½�
LCD_DATA_PORT|=command&0xf0; //å†™ä½Žå››ä½�

LCD_en_write();
}
//å†™æ•°æ�®
void LCD_write_data(unsigned char data)
{
delay_nus(16);
LCD_RS_PORT|=LCD_RS; //RS=1
LCD_DATA_PORT&=0X0f; //æ¸…é«˜å››ä½�
LCD_DATA_PORT|=data&0xf0; //å†™é«˜å››ä½�
//LCD_DATA_PORT = data; //å†™8ä½�

LCD_en_write();
data=data<<4; //ä½Žå››ä½�ç§»åˆ°é«˜å››ä½�
LCD_DATA_PORT&=0X0f; //æ¸…é«˜å››ä½�
LCD_DATA_PORT|=data&0xf0; //å†™ä½Žå››ä½�
LCD_en_write();
}
//å†™åœ°å�€å‡½æ•°
void LCD_set_xy( unsigned char x, unsigned char y )
{
unsigned char address;
if (y == 0) address = 0x80 + x;
else address = 0xc0 + x;
LCD_write_command( address);
}
//LCDåœ¨ä»»æ„�ä½�ç½®å†™å­—ç¬¦ä¸²
//åˆ—x=0~15,è¡Œy=0,1
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s)
{
	LCD_set_xy( X, Y ); //å†™åœ°å�€
while (*s) // å†™æ˜¾ç¤ºå­—ç¬¦
{
LCD_write_data( *s );
s ++;
}
}
//LCDåœ¨ä»»æ„�ä½�ç½®å†™å­—ç¬¦
//åˆ—x=0~15,è¡Œy=0,1
void LCD_write_char(unsigned char X,unsigned char Y,unsigned char data)
{
LCD_set_xy( X, Y ); //å†™åœ°å�€
LCD_write_data( data);
}
//å»¶æ—¶å‡½æ•°//////////////////////////////////////////////////////////////////////
//1uså»¶æ—¶å‡½æ•°
void delay_1us(void)
{
asm("nop");
}
//N uså»¶æ—¶å‡½æ•°
void delay_nus(unsigned int n)
{
unsigned int i;
for (i=0;i<n;i++)
delay_1us();
}
//1mså»¶æ—¶å‡½æ•°
void delay_1ms(void)
{
unsigned int i;
for (i=0;i<1140;i++);
}
//N mså»¶æ—¶å‡½æ•°
void delay_nms(unsigned int n)
{
unsigned int i=0;
for (i=0;i<n;i++)
delay_1ms();
}



#endif /* LCD1602_H_ */
