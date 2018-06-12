#include<msp430g2553.h>

#define CPU_F 8000000
#define delaynus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))
#define delaynms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))

#define TX_ADR_WIDTH 5   // 5  bytes TX(RX) address width

#define RX_ADR_WIDTH 5

#define TX_PLOAD_WIDTH 32  // 32 bytes TX payload

#define RX_PLOAD_WIDTH 32

typedef unsigned char BYTE;
typedef unsigned char uchar;
typedef unsigned int uint;


#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define STA_MARK_RX     0X40
#define STA_MARK_TX     0X20
#define STA_MARK_MX     0X10

#define IRQ             BIT5
#define CE              BIT0
#define CSN             BIT1
#define SCK             BIT2
#define MOSI            BIT3
#define MISO            BIT4

#define rIRQ P2IN&BIT5

#define CE_1 P2OUT|=BIT0
#define CE_0 P2OUT&=~BIT0

#define CSN_1 P2OUT|=BIT1
#define CSN_0 P2OUT&=~BIT1

#define SCK_1 P2OUT|=BIT2
#define SCK_0 P2OUT&=~BIT2

#define MOSI_1 P2OUT|=BIT3
#define MOSI_0 P2OUT&=~BIT3

#define rMISO P2IN&BIT4


uchar TX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static TX address

uchar RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};

uchar rx_buf[TX_PLOAD_WIDTH];

uchar tx_buf[TX_PLOAD_WIDTH];

uchar flag;

uchar st;

void nrf24l01_Init_IO(void);
void init_NRF24L01(void);
uchar SPI_RW(uchar byte);
uchar SPI_Read(uchar reg);
uchar SPI_RW_Reg(uchar reg,uchar value);
uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar bytes);
uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar uchars);
void SetTX_Mode(void);
void SetRX_Mode(void);
void NRF_TxPacket(uchar *tx_buf);
void NRF_RxPacket(uchar *rx_buf);


void nrf24l01_Init_IO(void){
	P2DIR|=CE+CSN+SCK+MOSI;
	P2DIR&=~(IRQ+MISO);
}

void init_NRF24L01(){
	delaynus(100);
	CE_0;
	CSN_1;
	SCK_0;
	SPI_Write_Buf(WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);
	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);
	SPI_RW_Reg(WRITE_REG+EN_AA,0x01);
	SPI_RW_Reg(WRITE_REG+EN_RXADDR,0x01);
	SPI_RW_Reg(WRITE_REG+SETUP_RETR,0x1a);
	SPI_RW_Reg(WRITE_REG+RF_CH,0x32);
	SPI_RW_Reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
	SPI_RW_Reg(WRITE_REG+RF_SETUP,0x0f);
	SPI_RW_Reg(WRITE_REG+CONFIG,0x7c);
}

uchar SPI_RW(uchar byte){
	char i,temp=0;
	for(i=0;i<8;i++){
		if((byte&0x80)==0x80)MOSI_1;
		else MOSI_0;
		byte=(byte<<1);
		temp<<=1;
		SCK_1;
		if(rMISO)temp++;
		SCK_0;
	}
	return (temp);
}

uchar SPI_Read(uchar reg){
	uchar reg_val;
	CSN_0;
	SPI_RW(reg);
	reg_val=SPI_RW(0);
	CSN_1;
	return (reg_val);
}

uchar SPI_RW_Reg(uchar reg,uchar value){
	uchar status;
	CSN_0;
	status=SPI_RW(reg);
	SPI_RW(value);
	CSN_1;
	return (status);
}

uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar bytes){
	uchar status,byte_ctr;
	CSN_0;
	status=SPI_RW(reg);
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)pBuf[byte_ctr]=SPI_RW(0);
	CSN_1;
	return (status);
}

uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar uchars){
	uchar status,uchar_ctr;
	CSN_0;
	status=SPI_RW(reg);
	for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)SPI_RW(*pBuf++);
	CSN_1;
	return (status);
}

void SetTX_Mode(){
	CE_0;
	SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	SPI_RW_Reg(FLUSH_TX,0x00);
	SPI_RW_Reg(WRITE_REG+CONFIG,0x7e);
	CE_1;
	delaynus(130);
}

void SetRX_Mode(){
	CE_0;
	SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	SPI_RW_Reg(FLUSH_RX,0x00);
	SPI_RW_Reg(WRITE_REG+CONFIG,0x7f);
	CE_1;
	delaynus(130);
}

void NRF_TxPacket(uchar *tx_buf){
	CE_0;
	SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,TX_ADDRESS,TX_ADR_WIDTH);
	SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);
	CE_1;
	delaynus(15);
	st=SPI_Read(STATUS);
	SPI_RW_Reg(WRITE_REG+STATUS,st);
}

void NRF_RxPacket(uchar *rx_buf){
	delaynus(10);
	st=SPI_Read(STATUS);
	SPI_RW_Reg(WRITE_REG+STATUS,st);
	if(st&STA_MARK_RX){
		CE_0;
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);
		SPI_RW_Reg(FLUSH_RX,0x00);
		CE_1;
	}
}


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

unsigned long int Temperature;
char Temp_str[16];


#define delay_nms(x) __delay_cycles((long)((double)CPU_F*x/1000.0))
#define delay_nus(x) __delay_cycles((long)((double)CPU_F*x/1000000.0))



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
  for(i=0;i<3;i++)
  {
    d *= 10;
    str[j++] = (int)d+'0';
    d -= (int)d;
  }
  while(str[--j]=='0');
    str[++j] = '\0';
  return str;
}



void main(void)
{
	WDTCTL=WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_8MHZ;                    // Set DCO
	DCOCTL = CALDCO_8MHZ;
	nrf24l01_Init_IO();
	init_NRF24L01();
	SetTX_Mode();
    float temp;
    int i;
	while(1){
		DS18B20_ReadTemp();
		temp=Temperature/10.0;
		TemperatureToString(temp,Temp_str);
		for(i=0;i<16;i++){
			tx_buf[i]=Temp_str[i];
		}
		NRF_TxPacket(tx_buf);
		delaynms(50);
	}
}


