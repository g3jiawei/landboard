
#ifndef __UART_APP_H__
#define __UART_APP_H__

#include <drivers/serial.h>
#include "drv_gpio.h"
#include "modbus.h"


#define MEAS_UART_NAME       "uart2"      /* 测量设备名称 */
#define COMM_UART_NAME       "uart3"      /* 通信设备名称 */


#define RS485_RE        GET_PIN(D, 7)
#define RS485_REEN(x)   do{ x ? rt_pin_write(RS485_RE, PIN_HIGH) : rt_pin_write(RS485_RE, PIN_LOW); }while(0)


/* 串口接收消息结构*/
struct rx_msg
{
    rt_device_t dev;
    rt_size_t size;
};

/* 串口发送消息结构*/
struct tx_msg
{
    rt_uint8_t *data_ptr;           //数据块的首地址
    rt_uint8_t data_size;           //数据的大小
};

/* 任务消息结构*/
//typedef struct 
//{
//    uint16_t  start_addr;               //起始地址
//    uint16_t  reg_num;                  //寄存器/线圈数量
//    uint8_t  cmdtype;                   //命令类型
//    uint8_t  dev_addr;                  //设备地址
//    uint8_t  function;                  //功能码
//    uint8_t  data_len;                  //数据字节数
//    uint8_t *data;                      //数据指针
//}task_msg;

typedef struct
{
    DecodedModbusBody body;
    uint16_t  cmdtype;                   //命令类型
}task_msg;

typedef task_msg *task_msg_t;

extern  uint8_t crx_buffer[RT_SERIAL_RB_BUFSZ];
extern  uint8_t ctx_buffer[RT_SERIAL_RB_BUFSZ];
extern  uint8_t mrx_buffer[RT_SERIAL_RB_BUFSZ];
extern  uint8_t mtx_buffer[RT_SERIAL_RB_BUFSZ];


/*定义通信口消息队列池大小*/
extern uint8_t crxmsg_pool[256];
extern uint8_t ctxmsg_pool[256];

/*定义测量口消息队列池大小*/
extern uint8_t mrxmsg_pool[128];
extern uint8_t mtxmsg_pool[128];

/*定义任务消息队列池大小*/
extern uint8_t taskmsg_pool[256];

/*定义串口设备串口2和串口3*/
extern rt_device_t serial2,serial3;

//static struct rx_msg crxmsg, mrxmsg;

extern struct rt_messagequeue crxmq,ctxmq,mrxmq,mtxmq,taskmq,taskcmdmq,taskaimmq;

    
/* 发送数据到通信发送消息队列 */
void send_com(uint8_t *data, rt_size_t length);
/* 发送数据到测量发送消息队列 */
void send_mea(void *data, rt_size_t length);


/* 串口参数初始化 */
void uart_app_init(void);
void rs485_init(void);
void msg_init(void);






#endif



