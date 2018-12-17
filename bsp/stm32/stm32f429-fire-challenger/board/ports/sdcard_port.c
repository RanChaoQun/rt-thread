/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-13     balanceTWK   add sdcard port file
 */

#include <rtthread.h>

#ifdef RT_USING_DFS

#include <dfs_elm.h>
#include <dfs_fs.h>
#include <dfs_posix.h>
#include "drv_gpio.h"

#define DBG_ENABLE
#define DBG_SECTION_NAME  "app.card"
#define DBG_COLOR

#define DBG_LEVEL DBG_INFO
#include <rtdbg.h>

#define WIFI_RESET_PIN                 GET_PIN(G,  9)

int ewm1062_disable(void)
{
    rt_pin_mode(WIFI_RESET_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(WIFI_RESET_PIN,PIN_LOW);
    return RT_EOK;
}
INIT_BOARD_EXPORT(ewm1062_disable);

void sd_mount(void *parameter)
{
    while (1)
    {
        rt_thread_mdelay(500);
        if(rt_device_find("sd0") != RT_NULL)
        {
            if (dfs_mount("sd0", "/", "elm", 0, 0) == RT_EOK)
            {
                LOG_I("sd card mount to '/'");
                break;
            }
            else
            {
                LOG_W("sd card mount to '/' failed!");
            }
        }
    }
}

int stm32_sdcard_mount(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("sd_mount", sd_mount, RT_NULL,
                           1024, RT_THREAD_PRIORITY_MAX - 2, 20);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("create sd_mount thread err!");
    }
    return RT_EOK;
}
INIT_APP_EXPORT(stm32_sdcard_mount);

#endif

