/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-02     Liuzeqi       the first version
 */

#ifdef RT_USING_FINSH
#include <finsh.h>
#endif

#include "led.h"


static Response_Led response_led[LED_MAX_NUM];


static int led_create(void)
{
    for (int i = 0; i < LED_MAX_NUM; i++)
    {
        if ( response_led[i].led == RT_NULL)
        {
            response_led[i].led_id = i;
            switch(i)
            {
                case 0:
                    response_led[i].led = agile_led_create(LED0_PIN, PIN_HIGH, "1000", -1);
                    break;
                case 1:
                    response_led[i].led = agile_led_create(LED1_PIN, PIN_HIGH, "1000", -1);
                    break;
            }
        }
        else
        {
            return RT_ERROR;
        }
    }
    return RT_EOK;
}

int led_start(int led_num)
{
    agile_led_t *led = RT_NULL;
    led = response_led[led_num].led;

    if(led == RT_NULL)
    {
        return RT_ERROR;
    }
    agile_led_on(led);
    return RT_EOK;
}



int led_stop(int led_num)
{
    agile_led_t *led = RT_NULL;
    led = response_led[led_num].led;

    if(led == RT_NULL)
    {
        return RT_ERROR;
    }
    agile_led_off(led);
    return RT_EOK;
}

int led_stop_all()
{
    for (int i = 0; i < 2; i++ )
    {
        led_stop(i);
    }
    return RT_EOK;
}

int tt(void)
{
    for(int i =0 ;i < 2; i++)
    {
        led_start(i);
        rt_thread_mdelay(1000);
        led_stop(i);
    }
    return RT_EOK;
}

INIT_APP_EXPORT(led_create);
MSH_CMD_EXPORT(tt,das);
