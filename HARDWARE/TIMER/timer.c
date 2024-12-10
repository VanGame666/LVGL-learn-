#include "timer.h"
#include "lvgl.h"

//////////////////////////////////////////////////////////////////////////////////	 
//WKS STM32F407VET6���İ�
//��ʱ���ж� ��������	   
//�汾��V1.0								  
////////////////////////////////////////////////////////////////////////////////// 	


TIM_HandleTypeDef TIM4_Handler;      	//��ʱ����� 
TIM_OC_InitTypeDef TIM4_CH1Handler;	//��ʱ��2ͨ��1���
//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪAPB2/2)

void TIM4_PWM_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                         	   //��ʱ��2
	TIM4_Handler.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
    TIM4_Handler.Init.Prescaler=psc;                         //��ʱ����Ƶϵ��
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;        //���ϼ���ģʽ
    TIM4_Handler.Init.Period=arr;                            //�Զ���װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	
    HAL_TIM_Base_Init(&TIM4_Handler);
	TIM4_Handler.State=HAL_TIM_STATE_RESET;
    HAL_TIM_PWM_Init(&TIM4_Handler);                         //��ʼ��PWM
    
    TIM4_CH1Handler.OCMode=TIM_OCMODE_PWM1;                  //ģʽѡ��PWM1
    TIM4_CH1Handler.Pulse=0;                             //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM4_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH;           //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH1Handler,TIM_CHANNEL_1);//����TIM2ͨ��2
	
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_1);//����PWMͨ��2
	HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   

}


//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //����ITM3�ж�   
	}
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM4_CLK_ENABLE();			      //TIM2ʱ��ʹ��
	__HAL_RCC_GPIOD_CLK_ENABLE();			      //����GPIOAʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_12;           	//PA1
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	  //�����������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	GPIO_Initure.Alternate= GPIO_AF2_TIM4;	//PA1����ΪTIM2_CH2
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
}

//��ʱ��3�жϷ�����

void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);
}

//�ص���������ʱ���жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM4_Handler))
    {
		lv_tick_inc(1);//lvgl��1ms�ж�
    }
}

void TIM_SetTIM4Compare1(u32 compare)
{
	TIM4->CCR1=compare; 
}







