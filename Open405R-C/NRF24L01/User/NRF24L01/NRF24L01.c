#include <stm32f4xx.h>
#include "NRF24L01.h"
#include "config.h"
#include <stdio.h>
#include <string.h>



#define	RX_DR			0x40
#define	TX_DS			0x20
#define	MAX_RT			0x10

u8 TX_ADDRESS[TX_ADR_WIDTH] = {0xb2,0xb2,0xb3,0xb4,0x01};  // 定义一个静态发送地址



u8 RX_BUF[TX_PLOAD_WIDTH];

u8 TX_BUF[TX_PLOAD_WIDTH];

/*配置MISO and MOSI SCLK 为复用功能（推挽）输出  Open_SPIx  */
/*配置SPI NRF24L01+片选	GPIOB_PIN_15     	CSN   	通用推挽输出模式 */
/*配置SPI NRF24L01+模式选择	GPIOB_PIN_14    CE		通用推挽输出模式*/
/*配置SPI NRF24L01+中断信号	 GPIOB_PIN_13   IRQ		上拉输入模式*/



static void Initial_SPI(SPI_TypeDef* SPIx)  //初始化IOB端口
{
	SPI_InitTypeDef SPI_InitStruct;	 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(Open_SPIx_SCK_GPIO_CLK | Open_SPIx_MISO_GPIO_CLK | Open_SPIx_MOSI_GPIO_CLK,ENABLE);

	RCC_APB2PeriphClockCmd(Open_SPIx_CLK,ENABLE);

	GPIO_PinAFConfig(Open_SPIx_SCK_GPIO_PORT, Open_SPIx_SCK_SOURCE,  Open_SPIx_SCK_AF);
	GPIO_PinAFConfig(Open_SPIx_MISO_GPIO_PORT, Open_SPIx_MISO_SOURCE, Open_SPIx_MISO_AF);
	GPIO_PinAFConfig(Open_SPIx_MOSI_GPIO_PORT, Open_SPIx_MOSI_SOURCE, Open_SPIx_MOSI_AF);
	
	GPIO_InitStructure.GPIO_Pin = Open_SPIx_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN; 

	GPIO_Init(Open_SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Open_SPIx_MISO_PIN;
	GPIO_Init(Open_SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Open_SPIx_MOSI_PIN;
	GPIO_Init(Open_SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

	SPI_I2S_DeInit(Open_SPIx);
	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low ;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;

	SPI_Init(Open_SPIx, &SPI_InitStruct);
	SPI_Cmd(Open_SPIx, ENABLE);

}

static void SPI_Send_byte(SPI_TypeDef* SPIx,u8 data)
{

	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPIx,data);

	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)==RESET);
	SPI_I2S_ReceiveData(SPIx);

}

static u8 SPI_Receive_byte(SPI_TypeDef* SPIx,u8 data)
{

	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPIx,data);

	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(SPIx);

}

static void delay1us(u8 t)
{
	while(--t);
} 

/****向寄存器reg写一个字节，同时返回状态字节**************/
u8 SPI_RW_Reg(u8 reg,u8 value)
{
	u8 status;
	CSN(0);
	status=SPI_Receive_byte(Open_SPIx,reg);   //select register  and write value to it
	SPI_Send_byte(Open_SPIx,value);   
	CSN(1);
	return(status); 
}
/****向寄存器reg读一个字节，同时返回状态字节**************/
u8 SPI_Read_Reg(u8 reg)
{
	u8 status;
	CSN(0);
	SPI_Send_byte(Open_SPIx,reg);
	status=SPI_Receive_byte(Open_SPIx,0);   //select register  and write value to it
	CSN(1);
	return(status);
}
/********读出bytes字节的数据*************************/
u8 SPI_Read_Buf(u8 reg,u8 *pBuf,u8 bytes)
{
	u8 status,byte_ctr;
	CSN(0);
	status=SPI_Receive_byte(Open_SPIx,reg);       
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		pBuf[byte_ctr]=SPI_Receive_byte(Open_SPIx,0);
	CSN(1);
	return(status);
}

/****************写入bytes字节的数据*******************/
u8 SPI_Write_Buf(u8 reg,u8 *pBuf,u8 bytes)
{
	u8 status,byte_ctr;
	CSN(0);
	status=SPI_Receive_byte(Open_SPIx,reg); 
	delay1us(1);
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		SPI_Send_byte(Open_SPIx,*pBuf++);
	CSN(1);
	return(status);
}
 /****************发送函数***************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE(0);			//StandBy I模式	
	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据	
	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
	CE(1);		 //置高CE，激发数据发送
	delay1us(10);
}

void RX_Mode(void)
{
	CE(0);
  	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 接收设备接收通道0使用和发送设备相同的发送地址
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RX_PW_P0, TX_PLOAD_WIDTH);  // 接收通道0选择和发送通道相同有效数据宽度 
 
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f);               // 使能接收通道0自动应答
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f);           // 使能接收通道0
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, 40);                 // 选择射频通道0x40

  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP, 0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0f);              // CRC使能，16位CRC校验，上电，接收模式
  	CE(1);
	delay1us(10);
}

void TX_Mode(u8 * tx_buf)
{
	CE(0);
  	SPI_Write_Buf(WRITE_REG_NRF24L01 + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
  	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
  	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f);       // 使能接收通道0自动应答
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f);   // 使能接收通道0
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_RETR, 0x0a);  // 自动重发延时等待250us+86us，自动重发10次
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, 40);         // 选择射频通道0x40
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP, 0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RX_PW_P0, TX_PLOAD_WIDTH);  // 接收通道0选择和发送通道相同有效数据宽度
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e);      // CRC使能，16位CRC校验，上电
	CE(1);
	delay1us(10);
} 

void nRF24L01_Initial(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(Open_CSN_GPIO_CLK | Open_CE_GPIO_CLK | Open_IRQ_GPIO_CLK ,ENABLE);
	/*CE CSN Initial*/
	GPIO_InitStructure.GPIO_Pin =Open_GPIO_Pin_CE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(Open_CE_GPIO, &GPIO_InitStructure);	

 	GPIO_InitStructure.GPIO_Pin =Open_GPIO_Pin_CSN;
	GPIO_Init(Open_CSN_GPIO, &GPIO_InitStructure);

	/*IRQ CSN Initial*/
	GPIO_InitStructure.GPIO_Pin = Open_GPIO_Pin_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(Open_IRQ_GPIO, &GPIO_InitStructure);

	Initial_SPI(Open_SPIx);

}

/****************** 配置函数********************************/
void nRF24L01_Config(void)
{
          //initial io
	//CE(0);          //        CE=0 ;chip enable
	//CSN(1);       //CSN=1   Spi disable
	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e); // Set PWR_UP bit, enable CRC(2 bytes) &Prim:RX. RX_DR enabled..
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f);
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f); // Enable Pipe0
//	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_AW, 0x02); // Setup address width=5 bytes
//	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, 40);
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP,0x07); // TX_PWR:0dBm, Datarate:2Mbps,
}
 
void NRF24L01_Send(void)
{
    u8 status=0x00;
	CE(0);
	TX_Mode(TX_BUF);
	while(IRQ);
	
	delay1us(10);
	status=SPI_Read_Reg(STATUS);	// 读取状态寄存其来判断数据接收状况
	printf("STATUS接受状态：0x%2x\r\n",status);
	if(status&TX_DS)	/*tx_ds == 0x20*/
	{
		printf("STATUS接受状态：0x%2x\r\n",status);
		printf("\r\n发送完数据：%s\r\n",RX_BUF);	
		SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x20);      // 清除TX，让IRQ拉低；
			
	}
	else if(status&MAX_RT)
		{
			printf("发送达到最大发送次数");	
			SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x10);      // 清除TX，让IRQ拉低；			
		}
	CE(1);

}

void NRF24L01_Receive(void)
{   
    u8 status=0x01;  
	CE(0);
	RX_Mode();
	while(IRQ);

	
	delay1us(10);
	status=SPI_Read_Reg(STATUS);	// 读取状态寄存其来判断数据接收状况
	printf("STATUS接受状态：0x%2x\r\n",status);
	if(status & 0x40)			//接受中断标志位
	{
		printf("接受成功");

		SPI_Read_Buf(RD_RX_PLOAD,RX_BUF,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer

		printf("\r\n 接收到数据：%x\r\n",RX_BUF[0]);
		SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x40);      // 清除TX，让IRQ拉低
	}  
	CE(1);

}









