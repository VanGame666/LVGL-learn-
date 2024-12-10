#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "timer.h"

/**
 * Create LED's with different brightness and color
 */
void lv_example_led_1(void)
{
    /*Create a LED and switch it OFF*/
    lv_obj_t * led1  = lv_led_create(lv_scr_act());
    lv_obj_align(led1, LV_ALIGN_CENTER, -80, 0);
    lv_led_off(led1);

    /*Copy the previous LED and set a brightness*/
    lv_obj_t * led2  = lv_led_create(lv_scr_act());
    lv_obj_align(led2, LV_ALIGN_CENTER, 0, 0);
    lv_led_set_brightness(led2, 150);
    lv_led_set_color(led2, lv_palette_main(LV_PALETTE_RED));

    /*Copy the previous LED and switch it ON*/
    lv_obj_t * led3  = lv_led_create(lv_scr_act());
    lv_obj_align(led3, LV_ALIGN_CENTER, 80, 0);
    lv_led_on(led3);
}


int main(void)
{ 
	
	HAL_Init();                   	//初始化HAL库    
	STM32_Clock_Init(336,25,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(115200);             	//初始化USART
//	TIM3_Init(1000-1,84-1);

	
	
	lv_init();			  // lvgl系统初始化
	lv_port_disp_init();  // lvgl显示接口初始化,放在lv_init()的后面
	lv_port_indev_init(); // lvgl输入接口初始化,放在lv_init()的后面
	TIM4_PWM_Init(1000-1,84-1); 

	//lv_example_led_1();
	lv_demo_keypad_encoder();
	TIM_SetTIM4Compare1(200);
	while (1)
	{
		lv_task_handler(); // lvgl的事务处理
	}

}

