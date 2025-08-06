/**
 ***********************************************************************************************
 * @file          usr_gpio.h
 * @author        
 * @version       
 * @date          
 * @brief         控电及LED对应gpio引脚的头文件定义
 * @license       Copyright (c) 2024-2048, 西安兰特水电测控技术有限责任公司
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

#define LED0_PIN        GET_PIN(B, 5)               /**< 红色LED */
#define LED1_PIN        GET_PIN(E, 5)               /**< 绿色LED */

#define PWR1_PIN        GET_PIN(G, 2)           /**< 控电引脚1 */
#define PWR2_PIN        GET_PIN(G, 3)           /**< 控电引脚2 */
#define PWR3_PIN        GET_PIN(G, 4)           /**< 控电引脚3 */
#define PWR4_PIN        GET_PIN(G, 5)           /**< 控电引脚4 */


void usr_gpio_init(void);                   /**< 控电GPIO口初始化 */
void power_on(rt_base_t pin);               /**< 给指定引脚上电 */
void power_off(rt_base_t pin);              /**< 给指定引脚断电 */

#endif
