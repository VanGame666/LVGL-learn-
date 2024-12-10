#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "sys.h"	
#include "touch.h"

//////////////////////////////////////////////////////////////////////////////////	 
//WKS STM32F407VET6核心板
//电容触摸屏-IIC 驱动代码
//版本：V1.0
////////////////////////////////////////////////////////////////////////////////// 	

//IO方向设置
#define CT_SDA_IN()  {GPIOB->MODER&=~(3<<(2*12));GPIOB->MODER|=0<<2*12;}	//PB12输入模式
#define CT_SDA_OUT() {GPIOB->MODER&=~(3<<(2*12));GPIOB->MODER|=1<<2*12;} 	//PB12输出模式
//IO操作函数	 
#define CT_IIC_SCL    PBout(15) 	//SCL
#define CT_IIC_SDA    PBout(12)   //SDA	 
#define CT_READ_SDA   PBin(12)    //输入SDA 
 

//IIC所有操作函数
void CT_IIC_Init(void);                	//初始化IIC的IO口				 
void CT_IIC_Start(void);				//发送IIC开始信号
void CT_IIC_Stop(void);	  				//发送IIC停止信号
void CT_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 CT_IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
u8 CT_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void CT_IIC_Ack(void);					//IIC发送ACK信号
void CT_IIC_NAck(void);					//IIC不发送ACK信号
u8 GT1151_WR_Reg(u16 reg,u8 *buf,u8 len);
void GT1151_RD_Reg(u16 reg,u8 *buf,u8 len); 
#endif







