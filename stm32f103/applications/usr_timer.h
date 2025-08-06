

#ifndef __USR_TIMER_H__
#define __USR_TIMER_H__

#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "rtthread.h"

/******************************************************************************************/
/* ������ʱ�� ���� */

/* TIMX �ж϶��� 
 * Ĭ�������TIM6/TIM7
 * ע��: ͨ���޸���4���궨��,����֧��TIM1~TIM8����һ����ʱ��.
 */
 
#define BTIM6_INT                       TIM6
#define BTIM6_INT_IRQn                  TIM6_DAC_IRQn
#define BTIM6_INT_IRQHandler            TIM6_DAC_IRQHandler
#define BTIM6_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM6_CLK_ENABLE(); }while(0)   /* TIM6 ʱ��ʹ�� */

#define BTIM7_INT                       TIM7
#define BTIM7_INT_IRQn                  TIM7_IRQn
#define BTIM7_INT_IRQHandler            TIM7_IRQHandler
#define BTIM7_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM7_CLK_ENABLE(); }while(0)   /* TIM7 ʱ��ʹ�� */


/******************************************************************************************/




/*
 *****************************************************************************************
 *  ��ʱʱ����㹫ʽ
 *  Tout = ��arr+1��*��psc+1��/ Ft
 *  arr��    ����Ŀ��ֵ
 *  psc��    Ԥ��Ƶϵ��
 *  Ft��     Ƶ�ʣ���ϵͳ��Ϊ72MHz
 *  Tout��   ��ʱ����ʱʱ��
 *  ����趨 arr = 4999��psc = 7199����Tout = 5000*7200/72000000 = 0.5s
 *  ��ʱʱ�䵽ִ�е��жϺ�����usr_timer.c�е�  void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)������
******************************************************************************************
*/
void btim6_int_init(uint16_t arr, uint16_t psc);    /* ������ʱ��6 ��ʱ�жϳ�ʼ������ */
void btim7_int_init(uint16_t arr, uint16_t psc);    /* ������ʱ��7 ��ʱ�жϳ�ʼ������ */


extern rt_timer_t stimer1,stimer2;

void stimeout1(void *parameter);

#endif

















