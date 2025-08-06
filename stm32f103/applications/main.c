/**
 ***********************************************************************************************
 * @file          main.c
 * @author        neale
 * @version       
 * @date          
 * @brief         
 * @license       Copyright (c) 2024-2048, 西安兰特水电测控技术有限责任公司
 **********************************************************************************************
 * @attention 
 * 
 * Modified Records
 * 
 ***********************************************************************************************
*/



#include "main.h"


int main(void)
{
    //rt_thread_t decodecom,sendcom,decodemea,sendmea,getdht11;

    rt_thread_t decodecom_thread,sendcom_thread,decodemea_thread,sendmea_thread,meamode_thread,measure_thread;
    

    msg_init();
    uart_app_init();         //com口通信初始化
    rs485_init();


    rtc_init();
    
    dht11_init();
    database_init();
    usr_gpio_init();
//    btim6_int_init(25000 - 1, 7200 - 1); /* 定时时间计算公式Tout = （arr+1）*（psc+1）/72000000  单位为s ，20000*7200/72000000 = 2 s */

//    btim7_int_init(10000 - 1, 7200 - 1);    
    
    /* 创建comrx线程 */
   decodecom_thread = rt_thread_create("comrx", decodecom_thread_entry, RT_NULL, 1024, 3, 10);
   
    /* 线程1创建成功则启动线程 */
    if (decodecom_thread != RT_NULL)
    {
        rt_thread_startup(decodecom_thread);
    }
    else
    {
        return RT_ERROR;
    }
    
    
    /* 创建comtx线程 */
    sendcom_thread = rt_thread_create("comtx", comsend_thread_entry, RT_NULL, 1024, 4, 10);
    if(sendcom_thread != RT_NULL){
        rt_thread_startup(sendcom_thread);
          }
        else
    {
        return RT_ERROR;
    }


    /* 创建mearx线程 */
   decodemea_thread = rt_thread_create("mearx", decodemea_thread_entry, RT_NULL, 1024, 3, 10);
   
    /* 线程创建成功则启动线程 */
    if (decodemea_thread != RT_NULL)
    {
        rt_thread_startup(decodemea_thread);
    }
    else
    {
        return RT_ERROR;
    }
    /* 创建meatx线程 */
    sendmea_thread = rt_thread_create("meatx", measend_thread_entry, RT_NULL, 1024, 4, 10);
    
        /* 线程创建成功则启动线程 */
    if (sendmea_thread != RT_NULL)
    {
        rt_thread_startup(sendmea_thread);
    }
    else
    {
        return RT_ERROR;
    }

        /* 创建measuremode线程 */
    meamode_thread = rt_thread_create("measuremode", meamode_thread_entry, RT_NULL, 1024, 4, 10);
    
        /* 线程创建成功则启动线程 */
    if (meamode_thread != RT_NULL)
    {
        rt_thread_startup(meamode_thread);
    }
    else
    {
        return RT_ERROR;
    }
    
            /* 创建measuremode线程 */
    measure_thread = rt_thread_create("measure", measure_thread_entry, RT_NULL, 1024, 4, 10);
    
        /* 线程创建成功则启动线程 */
    if (measure_thread != RT_NULL)
    {
        rt_thread_startup(measure_thread);
    }
    else
    {
        return RT_ERROR;
    }
    
    int num1 = 3;
    /* 创建定时器 1  周期定时器 */
    stimer1 = rt_timer_create("timer1", stimeout1,
                             &num1, 10000,
                             RT_TIMER_FLAG_PERIODIC);

    /* 启动定时器 1 */
    if (stimer1 != RT_NULL) rt_timer_start(stimer1);
    

    /*LED灯闪烁程序*/
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
        while (count++)
        {    
            rt_pin_write(LED0_PIN, PIN_HIGH);
            rt_pin_write(LED1_PIN, PIN_LOW);
            
            rt_thread_mdelay(500);
            rt_pin_write(LED0_PIN, PIN_LOW);
            rt_pin_write(LED1_PIN, PIN_HIGH);
            
            rt_thread_mdelay(500);
        }

        
}







