/*
 * main.c
 */
#include "msp430g2553.h"

unsigned char adc_num;    //  转换次数  转化10次求平均值
unsigned long voltage=0;    //  蓄电池电压*100   即值为1120时  表示11.20V
unsigned long vol;       //  电压的中间变量
unsigned long temp;       //   电压采集  寄存器中的数值
unsigned long i=0;       //定义循环变量


void delay(unsigned long x)
{
  unsigned long y,z;
  for(y=x;y>0;y--)
    for(z=600;z>0;z--);
}


void sys_init()    //开机蜂鸣器响三声
{
  P2OUT&=~BIT3;   //开启蜂鸣器
  delay(400);
  P2OUT|=BIT3;   //关闭蜂鸣器
  delay(400);

  P2OUT&=~BIT3;   //开启蜂鸣器
  delay(400);
  P2OUT|=BIT3;   //关闭蜂鸣器
  delay(400);

  P2OUT&=~BIT3;   //开启蜂鸣器
  delay(400);
  P2OUT|=BIT3;   //关闭蜂鸣器
}


void main()
{
  WDTCTL = WDTPW+WDTHOLD;    // 停止看门狗定时器
  P2SEL=0x00;      //  设置第二功能-通用IO口
  P2DIR|=0xff;
  P1SEL = 0x01;//将IO选择为AD输入
  P1DIR = 0;
  P2OUT|=BIT3;   //关闭蜂鸣器
  P2OUT|=BIT2;   //关闭LED灯


  BCSCTL1 |=CALBC1_1MHZ;
  DCOCTL |= CALDCO_1MHZ;
  BCSCTL2 |= SELM_0;//MCLK采用1M的内部DCO
  BCSCTL2 |= DIVS_2;//SMCLK采用250K的时钟

  sys_init();

  ADC10CTL0 |= SREF_1+REF2_5V+REFON+ADC10IE;       //将AD10基准设置为2.5V 开启AD允许中断
  ADC10CTL0 |= ADC10SHT_0+MSC;                    //打开AD转换，过采样率设置为4个采样周期
  ADC10CTL1 |= ADC10SSEL_3+SHS_0;                 //选择250K的采样时钟，用ADC10SC触发采集
  ADC10CTL1 |= CONSEQ_2+INCH_0;                  //采样模式，通道0
  ADC10CTL0 |= ADC10ON;
  ADC10AE0  |= 0xFF;
  ADC10CTL0 |= ADC10SC+ENC;
  _EINT();//允许中断

  while(1)
  {
    delay(300);
    ADC10CTL0 |=ADC10SC;
    for(adc_num=0;adc_num<10;adc_num++)
    {
      temp = ADC10MEM;
      vol = temp*250*6/1023;//计算采集到的电压
      voltage=voltage+vol;
    }
    voltage=voltage/10;
    if(voltage<900 ||voltage>850)
    {
        P2OUT&=~BIT2;   //开启LED灯
        for(i=0;i<10000;i++)
        {
          delay(300);
          P2OUT|=BIT3;   //关闭蜂鸣器
        }
    }
    else
    {
      if(voltage<851)
      {
        P2OUT&=~BIT2;   //开启LED灯
        for(i=0;i<10000;i++)
        {
          delay(1000);
          P2OUT|=BIT3;   //关闭蜂鸣器
        }
      }
      else
      {
        P2OUT|=BIT3;   //关闭蜂鸣器
        P2OUT|=BIT2;   //关闭LED灯
      }
    }
    voltage=0;
  }
}


//AD中断服务程序
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);          //将SR里的CPUOFF位置0
}

