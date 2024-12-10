#include "timer.h"
#include "lvgl.h"

//////////////////////////////////////////////////////////////////////////////////	 
//WKS STM32F407VET6核心板
//定时器中断 驱动代码	   
//版本：V1.0								  
////////////////////////////////////////////////////////////////////////////////// 	


TIM_HandleTypeDef TIM4_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef TIM4_CH1Handler;	//定时器2通道1句柄
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为APB2/2)

void TIM4_PWM_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                         	   //定时器2
	TIM4_Handler.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
    TIM4_Handler.Init.Prescaler=psc;                         //定时器分频系数
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;        //向上计数模式
    TIM4_Handler.Init.Period=arr;                            //自动重装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	
    HAL_TIM_Base_Init(&TIM4_Handler);
	TIM4_Handler.State=HAL_TIM_STATE_RESET;
    HAL_TIM_PWM_Init(&TIM4_Handler);                         //初始化PWM
    
    TIM4_CH1Handler.OCMode=TIM_OCMODE_PWM1;                  //模式选择PWM1
    TIM4_CH1Handler.Pulse=0;                             //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM4_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH;           //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH1Handler,TIM_CHANNEL_1);//配置TIM2通道2
	
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_1);//开启PWM通道2
	HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   

}


//定时器底层驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM3中断   
	}
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM4_CLK_ENABLE();			      //TIM2时钟使能
	__HAL_RCC_GPIOD_CLK_ENABLE();			      //开启GPIOA时钟
	
	GPIO_Initure.Pin=GPIO_PIN_12;           	//PA1
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	  //复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	GPIO_Initure.Alternate= GPIO_AF2_TIM4;	//PA1复用为TIM2_CH2
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
}

//定时器3中断服务函数

void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);
}

//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM4_Handler))
    {
		lv_tick_inc(1);//lvgl的1ms中断
    }
}

void TIM_SetTIM4Compare1(u32 compare)
{
	TIM4->CCR1=compare; 
}







