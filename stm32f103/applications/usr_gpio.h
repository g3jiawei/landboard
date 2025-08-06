/**
 ***********************************************************************************************
 * @file          usr_gpio.h
 * @author        
 * @version       
 * @date          
 * @brief         �ص缰LED��Ӧgpio���ŵ�ͷ�ļ�����
 * @license       Copyright (c) 2024-2048, ��������ˮ���ؼ����������ι�˾
 **********************************************************************************************
 * @attention 
 * 
 * Modified Records
 * 
 ***********************************************************************************************
*/

#ifndef     __USR_GPIO_H__
#define     __USR_GPIO_H__

#include <board.h>

#define LED0_PIN        GET_PIN(B, 5)               /**< ��ɫLED */
#define LED1_PIN        GET_PIN(E, 5)               /**< ��ɫLED */

#define PWR1_PIN        GET_PIN(G, 2)           /**< �ص�����1 */
#define PWR2_PIN        GET_PIN(G, 3)           /**< �ص�����2 */
#define PWR3_PIN        GET_PIN(G, 4)           /**< �ص�����3 */
#define PWR4_PIN        GET_PIN(G, 5)           /**< �ص�����4 */


void usr_gpio_init(void);                   /**< �ص�GPIO�ڳ�ʼ�� */
void power_on(rt_base_t pin);               /**< ��ָ�������ϵ� */
void power_off(rt_base_t pin);              /**< ��ָ�����Ŷϵ� */

#endif
