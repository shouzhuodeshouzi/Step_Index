/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-31     Liuzeqi       the first version
 */
#include <agile_button.h>
#include <drv_common.h>

#ifdef RT_USING_FINSH
#include <finsh.h>
#endif

#include "dev_common.h"


#include "protocol.h"
#include "uart_rcv.h"

#define DEBUG 1

#define LEFT_ID    0
#define RIGHT_ID   1
#define BTN_MAX    2

#define LEFT_PIN        GET_PIN(A, 6)    //  P1
#define RIGHT_PIN       GET_PIN(A, 4)    //  P2
#define VIP_PIN         GET_PIN(C, 1)


static agile_btn_t *left = RT_NULL;
static agile_btn_t *right = RT_NULL;
static agile_btn_t *vip = RT_NULL;

int send_footboard_stat(void)
{
    char *res_pkg;
    int res_id;
    if ( left_foot == 0 )
    {
        switch ( right_foot )
        {
        case 0:
            res_id = 0;
            break;
        case 1:
            res_id = 2;
            break;
        }
    }
    else
    {
        switch ( right_foot )
        {
        case 0:
            res_id = 1;
            break;
        case 1:
            res_id = 3;
            break;
        }
    }

    res_pkg = pt_pack(PACKAGE_WORK_DATA, 1, &res_id);
    rt_device_write(serial, 0, res_pkg, strlen(res_pkg));
    rt_free(res_pkg);

    return RT_EOK;
}

static void btn_click_event_cb(agile_btn_t *btn)
{
    char *res_pkg;

    if ( btn->button_id == BTN_MAX )
    {
        res_pkg = pt_pack(PACKAGE_STATE_DATA, 3, "00000000000000000000000000");
        rt_device_write(serial, 0, res_pkg, strlen(res_pkg));
        PT_FREE(res_pkg);
        send_footboard_stat();
    }

#if DEBUG
    rt_kprintf("[button click event] pin:%d   repeat:%d, hold_time:%d\r\n", btn->pin, btn->repeat_cnt, btn->hold_time);
    return ;
#endif

}

static void btn_press_down_event_cb(agile_btn_t *btn)
{
    switch ( btn->button_id )
    {
    case LEFT_ID:
        left_foot = 1;
        break;
    case RIGHT_ID:
        right_foot = 1;
        break;
    }
    send_footboard_stat();
#if DEBUG
    rt_kprintf("left_foot :%d   right_foot:%d\r\n", left_foot, right_foot);
    rt_kprintf("[button press down event] pin:%d   hold_time:%d\r\n", btn->pin, btn->hold_time);
#endif
}

static void btn_press_up_event_cb(agile_btn_t *btn)
{
    switch ( btn->button_id )
    {
    case LEFT_ID:
        left_foot = 0;
        break;
    case RIGHT_ID:
        right_foot = 0;
        break;
    }
    send_footboard_stat();
#if DEBUG
    rt_kprintf("left_foot :%d   right_foot:%d\r\n", left_foot, right_foot);
    rt_kprintf("[button press up event] pin:%d   hold_time:%d\r\n", btn->pin, btn->hold_time);
#endif
}

static int key_create(void)
{
    if(left == RT_NULL)
    {
        left = agile_btn_create(LEFT_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP,LEFT_ID);
        agile_btn_set_event_cb(left, BTN_PRESS_DOWN_EVENT, btn_press_down_event_cb);
        agile_btn_set_event_cb(left, BTN_PRESS_UP_EVENT, btn_press_up_event_cb);
        agile_btn_start(left);
    }

    if(right == RT_NULL)
    {
        right = agile_btn_create(RIGHT_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP,RIGHT_ID);
        agile_btn_set_event_cb(right, BTN_PRESS_DOWN_EVENT, btn_press_down_event_cb);
        agile_btn_set_event_cb(right, BTN_PRESS_UP_EVENT, btn_press_up_event_cb);
        agile_btn_start(right);
    }

    if(vip == RT_NULL)
    {
        vip = agile_btn_create(VIP_PIN, PIN_LOW, PIN_MODE_INPUT_PULLUP,BTN_MAX);
        agile_btn_set_event_cb(vip, BTN_CLICK_EVENT, btn_click_event_cb);
        agile_btn_start(vip);
    }
    return RT_EOK;
}


static int key_delete(void)
{
    if(left)
    {
        agile_btn_delete(left);
        left = RT_NULL;
    }

    if(right)
    {
        agile_btn_delete(right);
        right = RT_NULL;
    }

    if(vip)
    {
        agile_btn_delete(vip);
        vip= RT_NULL;
    }
    return RT_EOK;
}

#ifdef RT_USING_FINSH
MSH_CMD_EXPORT(send_footboard_stat,send footboard state);
MSH_CMD_EXPORT(key_create, create key);
MSH_CMD_EXPORT(key_delete, delete key);
#endif

INIT_APP_EXPORT(key_create);


