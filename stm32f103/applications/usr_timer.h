

#ifndef __USR_TIMER_H__
#define __USR_TIMER_H__

#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "rtthread.h"

/******************************************************************************************/
/* 基本定时器 定义 */

/* TIMX 中断定义 
 * 默认是针对TIM6/TIM7
 * 注意: 通过修改这4个宏定义,可以支持TIM1~TIM8任意一个定时器.
 */
 
#define BTIM6_INT                       TIM6
#define BTIM6_INT_IRQn                  TIM6_DAC_IRQn
#define BTIM6_INT_IRQHandler            TIM6_DAC_IRQHandler
#define BTIM6_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM6_CLK_ENABLE(); }while(0)   /* TIM6 时钟使能 */

#define BTIM7_INT                       TIM7
#define BTIM7_INT_IRQn                  TIM7_IRQn
#define BTIM7_INT_IRQHandler            TIM7_IRQHandler
#define BTIM7_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM7_CLK_ENABLE(); }while(0)   /* TIM7 时钟使能 */


/******************************************************************************************/




/*
 *****************************************************************************************
 *  定时时间计算公式
 *  Tout = （arr+1）*（psc+1）/ Ft
 *  arr：    计数目标值
 *  psc：    预分频系数
 *  Ft：     频率，本系统中为72MHz
 *  Tout：   定时器定时时间
 *  如果设定 arr = 4999，psc = 7199，则Tout = 5000*7200/72000000 = 0.5s
 *  定时时间到执行的中断函数在usr_timer.c中的  void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)函数中
******************************************************************************************
*/
void btim6_int_init(uint16_t arr, uint16_t psc);    /* 基本定时器6 定时中断初始化函数 */
void btim7_int_init(uint16_t arr, uint16_t psc);    /* 基本定时器7 定时中断初始化函数 */


extern rt_timer_t stimer1,stimer2;

void stimeout1(void *parameter);

#endif

















