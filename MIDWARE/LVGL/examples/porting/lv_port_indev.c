#if 1

#include "lv_port_indev.h"
#include "lvgl.h"
#include "touch.h"
#include "disp.h"


static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);


lv_indev_t * indev_touchpad;


void lv_port_indev_init(void)
{

    static lv_indev_drv_t indev_drv;

    touchpad_init();

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);


}



static void touchpad_init(void)
{
    /*Your code comes here*/
	GT1151_Init();
}



static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    if(touchpad_is_pressed())
	{
		touchpad_get_xy(&last_x, &last_y);
		data->state = LV_INDEV_STATE_PR;
    }else{

		data->state = LV_INDEV_STATE_REL;
    }
	
	data->point.x = last_x;
	data->point.y = last_y;
}


static bool touchpad_is_pressed(void)
{
    /*Your code comes here*/

    if (GT1151_IsPress())
    {
        return true;
    }

    return false;
}


static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/
	GT1151_GetPoint();
    (*x) = tp_dev.x[0];
    (*y) = tp_dev.y[0];
}


#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
