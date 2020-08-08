/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-01     Liuzeqi       the first version
 */
#ifndef APPLICATIONS_LED_H_
#define APPLICATIONS_LED_H_

#include <agile_led.h>
#include <drv_common.h>
#include <stdlib.h>

#define LED0_PIN      GET_PIN(A, 0)    //  P3
#define LED1_PIN      GET_PIN(C, 2)    //  P4

typedef enum LED_ID
{
    LED_0,
    LED_1,
    LED_MAX_NUM,
}LED_ID;

typedef struct response_led
{
    agile_led_t *led ;
    LED_ID led_id;
}Response_Led;




int led_start(int led_num);
int led_stop(int led_num);
int led_stop_all();

#endif /* APPLICATIONS_LED_H_ */
