
#ifndef __UART_APP_H__
#define __UART_APP_H__

#include <drivers/serial.h>
#include "drv_gpio.h"
#include "modbus.h"


#define MEAS_UART_NAME       "uart2"      /* �����豸���� */
#define COMM_UART_NAME       "uart3"      /* ͨ���豸���� */


#define RS485_RE        GET_PIN(D, 7)
#define RS485_REEN(x)   do{ x ? rt_pin_write(RS485_RE, PIN_HIGH) : rt_pin_write(RS485_RE, PIN_LOW); }while(0)


/* ���ڽ�����Ϣ�ṹ*/
struct rx_msg
{
    rt_device_t dev;
    rt_size_t size;
};

/* ���ڷ�����Ϣ�ṹ*/
struct tx_msg
{
    rt_uint8_t *data_ptr;           //���ݿ���׵�ַ
    rt_uint8_t data_size;           //���ݵĴ�С
};

/* ������Ϣ�ṹ*/
//typedef struct 
//{
//    uint16_t  start_addr;               //��ʼ��ַ
//    uint16_t  reg_num;                  //�Ĵ���/��Ȧ����
//    uint8_t  cmdtype;                   //��������
//    uint8_t  dev_addr;                  //�豸��ַ
//    uint8_t  function;                  //������
//    uint8_t  data_len;                  //�����ֽ���
//    uint8_t *data;                      //����ָ��
//}task_msg;

typedef struct
{
    DecodedModbusBody body;
    uint16_t  cmdtype;                   //��������
}task_msg;

typedef task_msg *task_msg_t;

extern  uint8_t crx_buffer[RT_SERIAL_RB_BUFSZ];
extern  uint8_t ctx_buffer[RT_SERIAL_RB_BUFSZ];
extern  uint8_t mrx_buffer[RT_SERIAL_RB_BUFSZ];
extern  uint8_t mtx_buffer[RT_SERIAL_RB_BUFSZ];


/*����ͨ�ſ���Ϣ���гش�С*/
extern uint8_t crxmsg_pool[256];
extern uint8_t ctxmsg_pool[256];

/*�����������Ϣ���гش�С*/
extern uint8_t mrxmsg_pool[128];
extern uint8_t mtxmsg_pool[128];

/*����������Ϣ���гش�С*/
extern uint8_t taskmsg_pool[256];

/*���崮���豸����2�ʹ���3*/
extern rt_device_t serial2,serial3;

//static struct rx_msg crxmsg, mrxmsg;

extern struct rt_messagequeue crxmq,ctxmq,mrxmq,mtxmq,taskmq,taskcmdmq,taskaimmq;

    
/* �������ݵ�ͨ�ŷ�����Ϣ���� */
void send_com(uint8_t *data, rt_size_t length);
/* �������ݵ�����������Ϣ���� */
void send_mea(void *data, rt_size_t length);


/* ���ڲ�����ʼ�� */
void uart_app_init(void);
void rs485_init(void);
void msg_init(void);






#endif



