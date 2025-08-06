/**
 ***********************************************************************************************
 * @file          main.c
 * @author        neale
 * @version       
 * @date          
 * @brief         
 * @license       Copyright (c) 2024-2048, ��������ˮ���ؼ����������ι�˾
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
    uart_app_init();         //com��ͨ�ų�ʼ��
    rs485_init();


    rtc_init();
    
    dht11_init();
    database_init();
    usr_gpio_init();
//    btim6_int_init(25000 - 1, 7200 - 1); /* ��ʱʱ����㹫ʽTout = ��arr+1��*��psc+1��/72000000  ��λΪs ��20000*7200/72000000 = 2 s */

//    btim7_int_init(10000 - 1, 7200 - 1);    
    
    /* ����comrx�߳� */
   decodecom_thread = rt_thread_create("comrx", decodecom_thread_entry, RT_NULL, 1024, 3, 10);
   
    /* �߳�1�����ɹ��������߳� */
    if (decodecom_thread != RT_NULL)
    {
        rt_thread_startup(decodecom_thread);
    }
    else
    {
        return RT_ERROR;
    }
    
    
    /* ����comtx�߳� */
    sendcom_thread = rt_thread_create("comtx", comsend_thread_entry, RT_NULL, 1024, 4, 10);
    if(sendcom_thread != RT_NULL){
        rt_thread_startup(sendcom_thread);
          }
        else
    {
        return RT_ERROR;
    }


    /* ����mearx�߳� */
   decodemea_thread = rt_thread_create("mearx", decodemea_thread_entry, RT_NULL, 1024, 3, 10);
   
    /* �̴߳����ɹ��������߳� */
    if (decodemea_thread != RT_NULL)
    {
        rt_thread_startup(decodemea_thread);
    }
    else
    {
        return RT_ERROR;
    }
    /* ����meatx�߳� */
    sendmea_thread = rt_thread_create("meatx", measend_thread_entry, RT_NULL, 1024, 4, 10);
    
        /* �̴߳����ɹ��������߳� */
    if (sendmea_thread != RT_NULL)
    {
        rt_thread_startup(sendmea_thread);
    }
    else
    {
        return RT_ERROR;
    }

        /* ����measuremode�߳� */
    meamode_thread = rt_thread_create("measuremode", meamode_thread_entry, RT_NULL, 1024, 4, 10);
    
        /* �̴߳����ɹ��������߳� */
    if (meamode_thread != RT_NULL)
    {
        rt_thread_startup(meamode_thread);
    }
    else
    {
        return RT_ERROR;
    }
    
            /* ����measuremode�߳� */
    measure_thread = rt_thread_create("measure", measure_thread_entry, RT_NULL, 1024, 4, 10);
    
        /* �̴߳����ɹ��������߳� */
    if (measure_thread != RT_NULL)
    {
        rt_thread_startup(measure_thread);
    }
    else
    {
        return RT_ERROR;
    }
    
    int num1 = 3;
    /* ������ʱ�� 1  ���ڶ�ʱ�� */
    stimer1 = rt_timer_create("timer1", stimeout1,
                             &num1, 10000,
                             RT_TIMER_FLAG_PERIODIC);

    /* ������ʱ�� 1 */
    if (stimer1 != RT_NULL) rt_timer_start(stimer1);
    

    /*LED����˸����*/
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







