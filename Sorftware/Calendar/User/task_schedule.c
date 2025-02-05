/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-01-19 14:20:38
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-01-19 14:28:23
 * @FilePath: \Calendar\User\task_schedule.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "task_schedule.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"


/**
 * @brief       task_schedule
 * @param       无
 * @retval      无
 */
void task_schedule(void)
{
    lcd_show_string(10, 10, 220, 32, 32, "STM32", RED);
    lcd_show_string(10, 47, 220, 24, 24, "Mem Manage", RED);
    lcd_show_string(10, 76, 220, 16, 16, "ATOM@ALIENTEK", RED);
    
    lcd_show_string(30, 118, 200, 16, 16, "Total Mem:      Bytes", RED);
    lcd_show_string(30, 139, 200, 16, 16, "Free  Mem:      Bytes", RED);
    lcd_show_string(30, 160, 200, 16, 16, "Malloc Addr:", RED);
    
    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       task1
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task1(void *pvParameters)
{
    uint8_t key         = 0;
    uint8_t *buf        = NULL;
    size_t  free_size   = 0;
    
    while (1)
    {
        key = key_scan(0);
        
        switch (key)
        {
            case KEY0_PRES:                                                 /* 申请内存和使用内存 */
            {
                buf = pvPortMalloc(30);
                sprintf((char *)buf, "0x%p", buf);
                lcd_show_string(130, 160, 200, 16, 16, (char *)buf, BLUE);
                break;
            }
            case KEY1_PRES:                                                 /* 释放内存 */
            {
                if (NULL != buf)
                {
                    vPortFree(buf);
                    buf = NULL;
                }
                break;
            }
            default:
            {
                break;
            }
        }
        
        lcd_show_xnum(114, 118, configTOTAL_HEAP_SIZE, 5, 16, 0, BLUE);     /* 显示总内存大小 */
        free_size = xPortGetFreeHeapSize();                                 /* 获取内存剩余大小 */
        lcd_show_xnum(114, 139, free_size, 5, 16, 0, BLUE);                 /* 显示剩余内存大小 */
        
        vTaskDelay(10);
    }
}
