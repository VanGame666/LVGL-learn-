#include "touch.h"

//////////////////////////////////////////////////////////////////////////////////	 
//WKS STM32F407VET6核心板
//电容触摸屏-GT1151 驱动代码	   
//版本：V1.0								  
////////////////////////////////////////////////////////////////////////////////// 

_m_tp_dev tp_dev=
{
	GT1151_Init,
	0,
	0,
	0,
	0, 
	0,
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};			
  



//初始化GT1151触摸屏
//返回值:0,初始化成功;1,初始化失败 
u8 GT1151_Init(void)
{	
	  u8 temp[5]; 
		GPIO_InitTypeDef GPIO_Initure;
	tp_dev.touchtype=0X00;
	tp_dev.scan=0;	//扫描函数指向GT1151触摸屏扫描
	tp_dev.touchtype|=0X80;			//电容屏 
	tp_dev.touchtype|=lcddev.dir&0X01;//横屏还是竖屏 
	  __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	

	
    //PB13
    GPIO_Initure.Pin=GPIO_PIN_13;           	//PB13设置为上拉输入
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化
            
    //PB14
    GPIO_Initure.Pin=GPIO_PIN_14;          	//PC14设置为推挽输出
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化
		CT_IIC_Init();      	//初始化电容屏的I2C总线  
		GT_RST=0;				//复位
		delay_ms(10);
		GT_RST=1;				//释放复位		    
		delay_ms(10); 
	
	  GPIO_Initure.Pin=GPIO_PIN_1;           	//PB1设置为上拉输入
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //输出模式
    GPIO_Initure.Pull=GPIO_NOPULL;          //无上下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化
		
	delay_ms(100);  
	GT1151_RD_Reg(GT_PID_REG,temp,4);//读取产品ID
	temp[4]=0;
	printf("CTP ID:%s\r\n",temp);	//打印ID
		if(strcmp((char*)temp,"1158")==0)//ID==1151
	{
		temp[0]=0X02;			
		GT1151_WR_Reg(GT_CTRL_REG,temp,1);//软复位GT1151
		GT1151_RD_Reg(GT_CFGS_REG,temp,1);//读取GT_CFGS_REG寄存器

		delay_ms(10);
		temp[0]=0X00;	 
		GT1151_WR_Reg(GT_CTRL_REG,temp,1);//结束复位   
		return 0;
	} 
	return 1;
}

const u16 GT1151_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};


u8 GT1151_IsPress()
{
	u8  regValue;

	GT1151_RD_Reg(GT_GSTID_REG,&regValue, 1);
	regValue = regValue & 0x0F;
	tp_dev.num = regValue;
	regValue = 0;
	GT1151_WR_Reg(GT_GSTID_REG,&regValue,1);
	if(tp_dev.num != 0){return 1;}
	else{return 0;}
}


void GT1151_GetPoint()
{
	u8  buf[4],i;
	
	for(i=0;i<1;i++)
	{
		if(tp_dev.num != 0)	//触摸有效?
		{
			GT1151_RD_Reg(GT1151_TPX_TBL[0],buf,4);	//读取XY坐标值
			tp_dev.x[0]=((u16)buf[1]<<8)+buf[0];
			tp_dev.y[0]=((u16)buf[3]<<8)+buf[2];
		}else{
			tp_dev.x[0]=0xffff;
		    tp_dev.y[0]=0xffff;
		}
	}
}















