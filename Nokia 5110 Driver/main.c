/*
 * main.c
 */
#include <msp430g2553.h>

#define CPU_F (double)12000000
#define delay_nms(x)  __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delay_nus(x)  __delay_cycles((long)((double)CPU_F*x/1000000.0))

typedef unsigned char uchar;
typedef unsigned int uint;

#define RST BIT3
#define RST_1 P2OUT|=BIT3
#define RST_0 P2OUT&=~BIT3

#define CE BIT4
#define CE_1 P2OUT|=BIT4
#define CE_0 P2OUT&=~BIT4

#define DC BIT5
#define DC_1 P2OUT|=BIT5
#define DC_0 P2OUT&=~BIT5

#define DIN BIT6
#define DIN_1 P1OUT|=BIT6
#define DIN_0 P1OUT&=~BIT6

#define CLK BIT7
#define CLK_1 P1OUT|=BIT7
#define CLK_0 P1OUT&=~BIT7


uchar map[42][6];

void N5110_WriteByte(uchar cmd,uchar data){
	uchar i;
	CE_0;
	if(cmd==0)DC_0;
	else DC_1;
	for(i=0;i<8;i++){
		if(data&0x80)DIN_1;
		else DIN_0;
		CLK_0;
		data=data<<1;
		CLK_1;
	}
	CE_1;
}

void N5110_SetXY(uchar x,uchar y){
	if(x>84)return;
	else N5110_WriteByte(0,0x80|x);
	if(y>6)return;
	else N5110_WriteByte(0,0x40|y);
}

void N5110_Clear(void){
	uint i;
	N5110_WriteByte(0,0x0c);
	N5110_WriteByte(0,0x80);
	N5110_SetXY(0,0);
	for(i=0;i<504;i++)N5110_WriteByte(1,0);

}

/*void N_DrawPoint(uchar x,uchar y){
	uchar y_temp;
	uchar carry;
	if(x>84||y>48)return;
	else {
		y_temp=y/8;
		if(y_temp==0){
			carry=y;
			N5110_SetXY(x,0);
			N5110_WriteByte(1,0x01<<carry);
		}
		else {
			carry=y-y_temp*8;
			N5110_SetXY(x,y_temp);
			N5110_WriteByte(1,0x01<<carry);
		}
	}
}*/

void N_DrawPoint(uchar x,uchar y){
	uchar mark_0,mark_1,mark_2,mark_3;
	uchar y_interval;
	if(x>41||y>23)return;
	else{
		y_interval=y/4;
		mark_0=(map[x][y_interval]&0x02)?0x03:0x00;
		mark_1=(map[x][y_interval]&0x08)?0x03:0x00;
		mark_2=(map[x][y_interval]&0x20)?0x03:0x00;
		mark_3=(map[x][y_interval]&0x80)?0x03:0x00;
		if(y_interval==0){
			N5110_SetXY(x*2,0);
			N5110_WriteByte(1,(mark_3<<6)|(mark_2<<4)|(mark_1<<2)|(mark_0));
			N5110_SetXY(x*2+1,0);
			N5110_WriteByte(1,(mark_3<<6)|(mark_2<<4)|(mark_1<<2)|(mark_0));
		}
		else{
			N5110_SetXY(x*2,y_interval);
			N5110_WriteByte(1,(mark_3<<6)|(mark_2<<4)|(mark_1<<2)|(mark_0));
			N5110_SetXY(x*2+1,y_interval);
			N5110_WriteByte(1,(mark_3<<6)|(mark_2<<4)|(mark_1<<2)|(mark_0));
		}
	}
}


void N5110_Init(void){
	P1DIR|=BIT6+BIT7;
	P2DIR|=BIT3+BIT4+BIT5;
	RST_0;
	delay_nus(10);
	RST_1;
	CE_0;
	delay_nus(10);
	CE_1;
	delay_nus(10);
	N5110_WriteByte(0,0x21);//使用扩展指令
	N5110_WriteByte(0,0xd0);//调整偏置电压
	//N5110_WriteByte(0,0x06);//温度校正
	N5110_WriteByte(0,0x13);//
	N5110_WriteByte(0,0x20);//水平寻址
	N5110_Clear();
	N5110_WriteByte(0,0x0c);//显示模式
	CE_0;

}

void Mapping_VRAM_Write(uchar x,uchar y,uchar value){
	uchar y_interval;
	uchar carry;
	y_interval=y/4;
	carry=y-y_interval*4;
	carry*=2;
	if(value>=1){
		if(carry==0)map[x][y_interval]|=0x03;
		if(carry==2)map[x][y_interval]|=0x0C;
		if(carry==4)map[x][y_interval]|=0x30;
		if(carry==6)map[x][y_interval]|=0xC0;
	}
	else{
		if(carry==0)map[x][y_interval]&=0xFC;
		if(carry==2)map[x][y_interval]&=0xF3;
		if(carry==4)map[x][y_interval]&=0xCF;
		if(carry==6)map[x][y_interval]&=0x3F;
	}
}



void Frame_Refresh(void){
	int i,j;
	for(i=0;i<42;i++)
		for(j=0;j<24;j++)N_DrawPoint(i,j);
}



void Wall_Display(void){
	   int i;
		for(i=0;i<42;i++){
		    Mapping_VRAM_Write(i,0,1);
		    N_DrawPoint(i,0);
		}
		for(i=0;i<24;i++){
			Mapping_VRAM_Write(41,i,1);
			N_DrawPoint(41,i);
		}
		for(i=41;i>=0;i--){
		    Mapping_VRAM_Write(i,23,1);
		    N_DrawPoint(i,23);
		}
		for(i=23;i>=0;i--){
		    Mapping_VRAM_Write(0,i,1);
		    N_DrawPoint(0,i);
		}
}


void Key_Init(void){
	P1DIR&=~BIT4+BIT5;
	P2DIR&=~BIT0+BIT1+BIT2;
	P1REN|=BIT4+BIT5;
	P2REN|=BIT0+BIT1+BIT2;
	P1OUT|=BIT4+BIT5;
	P2OUT|=BIT0+BIT1+BIT2;
	P1IES|=BIT4+BIT5;
	P2IES|=BIT0+BIT1+BIT2;
	P1IFG&=~BIT4+BIT5;
	P2IFG&=~BIT0+BIT1+BIT2;
	P1IE|=BIT4+BIT5;
	P2IE|=BIT0+BIT1+BIT2;
}

uchar start=0;
unsigned int rand_seed;

typedef struct Snake{
	uchar x;
	uchar y;
	uchar direction;
}Snake;

const int direct[4]={-1,1,1,-1};

typedef struct Food{
	uchar x;
	uchar y;
	uchar eaten;
}Food;

Food food={0,0,1};
uchar snake_len=1;
Snake s[20];


uchar rand(void){
	return rand_seed;
}

void Game_Init(void){
	Wall_Display();
}

void feeding(void){
	food.x=rand()%39+1;
	food.y=rand()%21+1;
	food.eaten=0;
}

void Running(void){
	int i;
	if(food.eaten==1)feeding();
	Mapping_VRAM_Write(food.x,food.y,1);
	if(s[0].direction==0||s[0].direction==2)s[0].y=s[0].y+direct[s[0].direction];
	else s[0].x=s[0].x+direct[s[0].direction];
	Mapping_VRAM_Write(s[0].x,s[0].y,1);
	if(s[0].x==0||s[0].x==41||s[0].y==0||s[0].y==23)start=0;
	if(s[0].x==food.x&&s[0].y==food.y){
					food.eaten=1;
					snake_len++;
				}
	if(food.eaten==0){
		for(i=1;i<snake_len;i++){
			if(i==1)Mapping_VRAM_Write(s[i].x,s[i].y,1);
			if(i==snake_len-1){
				s[i].x=s[0].x;
				s[i].y=s[0].y;
				s[i].direction=s[0].direction;
			}
			else{
				s[i].x=s[i+1].x;
				s[i].y=s[i+1].y;
			    s[i].direction=s[i+1].direction;
			}
			Mapping_VRAM_Write(s[i].x,s[i].y,1);
		}
	}
}

void Frame_Clear(void){
	int i,j;
	for(i=1;i<41;i++)
		for(j=1;j<23;j++)Mapping_VRAM_Write(i,j,0);
	Frame_Refresh();
}


void main(void) {
	WDTCTL=WDTPW+WDTHOLD;
	BCSCTL1 = CALBC1_12MHZ;                    // Set DCO
	DCOCTL = CALDCO_12MHZ;
	CCTL0=CCIE;
	TACTL=TASSEL_2+MC_2;
	P1DIR|=BIT2;
	P1OUT&=~BIT2;
	Key_Init();
	N5110_Init();
	__bis_SR_register(GIE);
	Game_Init();
	s[0].x=37;
	s[0].y=12;
	while(1){
		while(start){
			Frame_Clear();
			Running();
			Frame_Refresh();
		}
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
	if(P1IFG&BIT4){
		start^=0x01;
		P1IFG&=~BIT4;
	}
	else if(P1IFG&BIT5){
				delay_nms(100);
				s[0].direction=3;
				s[0].x+=direct[s[0].direction];
				Mapping_VRAM_Write(s[0].x,s[0].y,1);
				P1IFG&=~BIT5;
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){
	if(P2IFG&BIT0){
		delay_nms(100);
		s[0].direction=1;
		s[0].x+=direct[s[0].direction];
		Mapping_VRAM_Write(s[0].x,s[0].y,1);
		P2IFG&=~BIT0;
	}
	else if(P2IFG&BIT1){
		delay_nms(100);
		s[0].direction=2;
		s[0].y+=direct[s[0].direction];
		Mapping_VRAM_Write(s[0].x,s[0].y,1);
		P2IFG&=~BIT1;
	}
	else if(P2IFG&BIT2){
		delay_nms(100);
		s[0].direction=0;
		s[0].y+=direct[s[0].direction];
		Mapping_VRAM_Write(s[0].x,s[0].y,1);
		P2IFG&=~BIT2;
	}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
	rand_seed+=1;
}


