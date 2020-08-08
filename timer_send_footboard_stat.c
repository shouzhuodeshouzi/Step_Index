/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-06     Liuzeqi       the first version
 */
/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-06     Liuzeqi       the first version
 */
#include <rtdef.h>
#include <rtthread.h>
#include "dev_common.h"
#include "protocol.h"
#include "uart_rcv.h"
#include "string.h"

char res_pkg[128];
int res_id = 23;

static void timeout_callbak_sd_ft_stat(void *parameter)
{


    if (dev_stat == 2 )
    {
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

        rt_snprintf(res_pkg, 128, "{%d%.2d%.2d%d[%d]}",PACKAGE_WORK_DATA, dev_type, dev_id, 1, res_id);
        rt_device_write(serial, 0, res_pkg, strlen(res_pkg));
        memset(res_pkg, '\0', 128);
    }
}

static int send_footboard_stat(void)
{
    rt_timer_t timer_ftbd_stat;
    timer_ftbd_stat = rt_timer_create("send_footboard_stat", timeout_callbak_sd_ft_stat, RT_NULL, 500, RT_TIMER_FLAG_PERIODIC);
    if ( timer_ftbd_stat != RT_NULL )
    {
        rt_timer_start(timer_ftbd_stat);
    }

    return RT_EOK;
}

INIT_APP_EXPORT(send_footboard_stat);
