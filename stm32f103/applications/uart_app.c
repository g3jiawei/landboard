/**
 ***********************************************************************************************
 * @file          uart_app.c
 * @author        neale
 * @version       V1.0
 * @date          24/07/2024
 * @brief         建立所有的消息队列，以及通信回调函数
 * @license       Copyright (c) 2024-2048, 西安兰特水电测控技术有限责任公司
 **********************************************************************************************
 * @attention 
 * 
 * Modified Records
 * 
 ***********************************************************************************************
*/

#include "uart_app.h"
#include <rtdevice.h>

uint8_t crx_buffer[RT_SERIAL_RB_BUFSZ];
uint8_t ctx_buffer[RT_SERIAL_RB_BUFSZ];
uint8_t mrx_buffer[RT_SERIAL_RB_BUFSZ];
uint8_t mtx_buffer[RT_SERIAL_RB_BUFSZ];


/*定义通信口消息队列池大小*/
 uint8_t crxmsg_pool[256];
 uint8_t ctxmsg_pool[256];

/*定义测量口消息队列池大小*/
 uint8_t mrxmsg_pool[128];
 uint8_t mtxmsg_pool[128];
 
 uint8_t taskmsg_pool[256];
 uint8_t task_cmdmsg_pool[128];
 uint8_t task_aimmsg_pool[128];
/*定义串口设备串口2和串口3*/

rt_device_t serial2,serial3;

struct rt_messagequeue crxmq,ctxmq,mrxmq,mtxmq,taskmq,taskcmdmq,taskaimmq;


/*消息队列初始化*/
void msg_init(void){
               /* 初始化通信接收队列 */    
    rt_mq_init(&crxmq, "crxmq",
               crxmsg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct rx_msg),    /* 一条消息的最大长度 */
               sizeof(crxmsg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

               /* 初始化通信发送队列 */
    rt_mq_init(&ctxmq, "ctxmq",
               ctxmsg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct tx_msg),    /* 一条消息的最大长度 */
               sizeof(ctxmsg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */
    
               /* 初始化测量接收队列 */
    rt_mq_init(&mrxmq, "meas_rxmq",
               mrxmsg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct rx_msg),    /* 一条消息的最大长度 */
               sizeof(mrxmsg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */
    
               /* 初始化测量发送队列 */
    rt_mq_init(&mtxmq, "meas_txmq",
               mtxmsg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct tx_msg),    /* 一条消息的最大长度 */
               sizeof(mrxmsg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

               /* 初始化任务消息队列 */
    rt_mq_init(&taskmq, "task_mq",
               taskmsg_pool,                 /* 存放消息的缓冲区 */
               sizeof(task_msg),    /* 一条消息的最大长度 */
               sizeof(taskmsg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */
               
    rt_mq_init(&taskcmdmq, "taskcmd_mq",
               task_cmdmsg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct rx_msg),           /* 一条消息的最大长度 */
               sizeof(task_cmdmsg_pool),        /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */
    rt_mq_init(&taskaimmq, "taskaim_mq",
               task_aimmsg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct rx_msg),           /* 一条消息的最大长度 */
               sizeof(task_aimmsg_pool),        /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */
}

/*通信接口函数定义*/


/*
********************************************************************************
* 
* 功    能：通信口接收数据回调函数，接收完成发送接收数据到消息队列crxmq
* 入口参数：rt_device_t dev             串口设备名称
*           rt_size_t size              数据长度
* 出口参数：rt_err_t result
* 描    述：
*           返回值 == 0，操作成功；其它则操作失败
********************************************************************************
*/
/**
 **********************************************************************
 * @brief      通信口接收数据回调函数，接收完成发送接收数据到消息队列crxmq
 * @param[in]      rt_device_t dev             串口设备名称
 * @param[in]      rt_size_t size              数据长度
 * @retval     
 **********************************************************************
 */

rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    struct rx_msg crxmsg;
    rt_err_t result;
    crxmsg.dev = dev;
    crxmsg.size = size;

    result = rt_mq_send(&crxmq, &crxmsg, sizeof(crxmsg));
    if ( result == -RT_EFULL)
    {
        /* 消息队列满 */
        rt_kprintf("message queue full！\n");
    }
    return result;
}
/*
********************************************************************************
* 功    能：通信口初始化函数
* 入口参数：
* 出口参数：
* 描    述：    1、查找串口设备
*               2、修改设备参数
*               3、设置接口回调函数  uart_input
********************************************************************************
*/

void uart_app_init(void){
     
    /* 串口设备句柄 */

    
    char uart_name[RT_NAME_MAX];
    
    struct serial_configure config3 = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */
    
    char str[] = "uart3 init success!\r\n";
    rt_strncpy(uart_name, COMM_UART_NAME, RT_NAME_MAX);
    
    /* step1：查找串口设备 */
    serial3 = rt_device_find(uart_name);
    if (!serial3)
    {
        rt_kprintf("find %s failed!\n", uart_name);
    }
    /* step2：修改串口配置参数 */
    config3.baud_rate = BAUD_RATE_9600;        //修改波特率为 9600
    config3.data_bits = DATA_BITS_8;           //数据位 8
    config3.stop_bits = STOP_BITS_1;           //停止位 1
    config3.bufsz     = 128;                   //修改缓冲区 buff size 为 128
    config3.parity    = PARITY_NONE;           //无奇偶校验位
    
    /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
    rt_device_control(serial3, RT_DEVICE_CTRL_CONFIG, &config3);
    
    /* 以 DMA 接收及轮询发送方式打开串口设备 */
    rt_device_open(serial3, RT_DEVICE_FLAG_DMA_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial3, uart_input);
        /* 发送字符串 */
    rt_device_write(serial3, 0, str, (sizeof(str) - 1));

}


void send_com(uint8_t *data, rt_size_t length)
{    struct tx_msg msg;

    msg.data_ptr = data;  /* 指向相应的数据块地址 */
    msg.data_size = length; /* 数据块的长度 */

    /* 发送这个消息指针给 mq 消息队列 */

    rt_mq_send(&ctxmq, (void*)&msg, sizeof(msg));
}







/*测量接口函数定义*/


/*
********************************************************************************
* 
* 功    能：测量口接收数据回调函数，接收完成发送接收数据到消息队列mrxmq
* 入口参数：rt_device_t dev             串口设备名称
*           rt_size_t size              数据长度
* 出口参数：rt_err_t result
* 描    述：
*           返回值 == 0，操作成功；其它则操作失败
********************************************************************************
*/
rt_err_t rs485_input(rt_device_t dev, rt_size_t size)
{
    struct rx_msg mrxmsg;
    rt_err_t result;
    mrxmsg.dev = dev;
    mrxmsg.size = size;

    result = rt_mq_send(&mrxmq, &mrxmsg, sizeof(mrxmsg));
    if ( result == -RT_EFULL)
    {
        /* 消息队列满 */
        rt_kprintf("mrxmq message queue full！\n");
    }
    return result;
}
/*
********************************************************************************
* 功    能：测量口初始化函数
* 入口参数：
* 出口参数：
* 描    述：    1、查找串口设备
*               2、修改设备参数
*               3、设置接口回调函数  uart_input
********************************************************************************
*/

void rs485_init(void){
    
    char uart_name[RT_NAME_MAX];
    
    struct serial_configure config2 = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */
    
    char str[] = "uart2 init success!\r\n";
    
    rt_strncpy(uart_name, MEAS_UART_NAME, RT_NAME_MAX);

    rt_pin_mode(RS485_RE, PIN_MODE_OUTPUT);
    
    /* step1：查找串口设备 */
    serial2 = rt_device_find(uart_name);
    if (!serial2)
    {
        rt_kprintf("find %s failed!\n", uart_name);
    }
    /* step2：修改串口配置参数 */
    config2.baud_rate = BAUD_RATE_9600;        //修改波特率为 9600
    config2.data_bits = DATA_BITS_8;           //数据位 8
    config2.stop_bits = STOP_BITS_1;           //停止位 1
    config2.bufsz     = 128;                   //修改缓冲区 buff size 为 128
    config2.parity    = PARITY_NONE;           //无奇偶校验位
    
    /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
    rt_device_control(serial2, RT_DEVICE_CTRL_CONFIG, &config2);
    
    /* 以 DMA 接收及轮询发送方式打开串口设备 */
    rt_device_open(serial2, RT_DEVICE_FLAG_DMA_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial2, rs485_input);
        /* 发送字符串 */
    RS485_REEN(1); 
    rt_device_write(serial2, 0, str, (sizeof(str) - 1));
    RS485_REEN(0); 

}

void send_mea(void *data, rt_size_t length)
{    struct tx_msg msg;

    msg.data_ptr = data;  /* 指向相应的数据块地址 */
    msg.data_size = length; /* 数据块的长度 */

    /* 发送这个消息指针给 mq 消息队列 */
    rt_mq_send(&mtxmq, (void*)&msg, sizeof(msg));
}




