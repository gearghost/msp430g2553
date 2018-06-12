/*
 * main.c
 */
#include <msp430g2553.h>
#include <intrinsics.h>
#include "lcd1602.h"
void main(void) {
	WDTCTL=WDTHOLD+WDTPW;
	LCD_init();
	delay_1ms();
	LCD_write_string(0,0,"Hello,BlueTooth!");
	delay_1ms();
}
