/*
*********************************************************************************************************
*
*    模块名称 : 头文件汇总
*    文件名称 : includes.h
*    版    本 : V1.0
*    说    明 : 当前使用头文件汇总
*
*    修改记录 :
*        版本号    日期        作者     说明
*        V1.0    2015-08-02  Eric2013   首次发布
*
*    Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__



/*
*********************************************************************************************************
*                                         标准库
*********************************************************************************************************
*/
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                          OS和系统库
*********************************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"
#include "event_groups.h"
#include <Net_Config.h>


/*
*********************************************************************************************************
*                                           宏定义
*********************************************************************************************************
*/
/* 给TCPnet任务发的事件标志 */
#define KEY1_BIT0       (1 << 0)
#define KEY2_BIT1       (1 << 1)
#define KEY3_BIT2       (1 << 2)
#define JOY_U_BIT3      (1 << 3)
#define JOY_L_BIT4      (1 << 4)
#define JOY_R_BIT5      (1 << 5)



/*
*********************************************************************************************************
*                                        APP / BSP
*********************************************************************************************************
*/
#include  <bsp.h>


/*
*********************************************************************************************************
*                                          变量和函数
*********************************************************************************************************
*/
extern EventGroupHandle_t xCreatedEventGroup;
extern void TCPnetTest(void);

/* 在主函数中调用 */
extern void vSetupSysInfoTest(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
