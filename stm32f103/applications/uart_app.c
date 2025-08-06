/**
 ***********************************************************************************************
 * @file          uart_app.c
 * @author        neale
 * @version       V1.0
 * @date          24/07/2024
 * @brief         �������е���Ϣ���У��Լ�ͨ�Żص�����
 * @license       Copyright (c) 2024-2048, ��������ˮ���ؼ����������ι�˾
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


/*����ͨ�ſ���Ϣ���гش�С*/
 uint8_t crxmsg_pool[256];
 uint8_t ctxmsg_pool[256];

/*�����������Ϣ���гش�С*/
 uint8_t mrxmsg_pool[128];
 uint8_t mtxmsg_pool[128];
 
 uint8_t taskmsg_pool[256];
 uint8_t task_cmdmsg_pool[128];
 uint8_t task_aimmsg_pool[128];
/*���崮���豸����2�ʹ���3*/

rt_device_t serial2,serial3;

struct rt_messagequeue crxmq,ctxmq,mrxmq,mtxmq,taskmq,taskcmdmq,taskaimmq;


/*��Ϣ���г�ʼ��*/
void msg_init(void){
               /* ��ʼ��ͨ�Ž��ն��� */    
    rt_mq_init(&crxmq, "crxmq",
               crxmsg_pool,                 /* �����Ϣ�Ļ����� */
               sizeof(struct rx_msg),    /* һ����Ϣ����󳤶� */
               sizeof(crxmsg_pool),         /* �����Ϣ�Ļ�������С */
               RT_IPC_FLAG_FIFO);        /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */

               /* ��ʼ��ͨ�ŷ��Ͷ��� */
    rt_mq_init(&ctxmq, "ctxmq",
               ctxmsg_pool,                 /* �����Ϣ�Ļ����� */
               sizeof(struct tx_msg),    /* һ����Ϣ����󳤶� */
               sizeof(ctxmsg_pool),         /* �����Ϣ�Ļ�������С */
               RT_IPC_FLAG_FIFO);        /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
    
               /* ��ʼ���������ն��� */
    rt_mq_init(&mrxmq, "meas_rxmq",
               mrxmsg_pool,                 /* �����Ϣ�Ļ����� */
               sizeof(struct rx_msg),    /* һ����Ϣ����󳤶� */
               sizeof(mrxmsg_pool),         /* �����Ϣ�Ļ�������С */
               RT_IPC_FLAG_FIFO);        /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
    
               /* ��ʼ���������Ͷ��� */
    rt_mq_init(&mtxmq, "meas_txmq",
               mtxmsg_pool,                 /* �����Ϣ�Ļ����� */
               sizeof(struct tx_msg),    /* һ����Ϣ����󳤶� */
               sizeof(mrxmsg_pool),         /* �����Ϣ�Ļ�������С */
               RT_IPC_FLAG_FIFO);        /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */

               /* ��ʼ��������Ϣ���� */
    rt_mq_init(&taskmq, "task_mq",
               taskmsg_pool,                 /* �����Ϣ�Ļ����� */
               sizeof(task_msg),    /* һ����Ϣ����󳤶� */
               sizeof(taskmsg_pool),         /* �����Ϣ�Ļ�������С */
               RT_IPC_FLAG_FIFO);        /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
               
    rt_mq_init(&taskcmdmq, "taskcmd_mq",
               task_cmdmsg_pool,                 /* �����Ϣ�Ļ����� */
               sizeof(struct rx_msg),           /* һ����Ϣ����󳤶� */
               sizeof(task_cmdmsg_pool),        /* �����Ϣ�Ļ�������С */
               RT_IPC_FLAG_FIFO);        /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
    rt_mq_init(&taskaimmq, "taskaim_mq",
               task_aimmsg_pool,                 /* �����Ϣ�Ļ����� */
               sizeof(struct rx_msg),           /* һ����Ϣ����󳤶� */
               sizeof(task_aimmsg_pool),        /* �����Ϣ�Ļ�������С */
               RT_IPC_FLAG_FIFO);        /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
}

/*ͨ�Žӿں�������*/


/*
********************************************************************************
* 
* ��    �ܣ�ͨ�ſڽ������ݻص�������������ɷ��ͽ������ݵ���Ϣ����crxmq
* ��ڲ�����rt_device_t dev             �����豸����
*           rt_size_t size              ���ݳ���
* ���ڲ�����rt_err_t result
* ��    ����
*           ����ֵ == 0�������ɹ������������ʧ��
********************************************************************************
*/
/**
 **********************************************************************
 * @brief      ͨ�ſڽ������ݻص�������������ɷ��ͽ������ݵ���Ϣ����crxmq
 * @param[in]      rt_device_t dev             �����豸����
 * @param[in]      rt_size_t size              ���ݳ���
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
        /* ��Ϣ������ */
        rt_kprintf("message queue full��\n");
    }
    return result;
}
/*
********************************************************************************
* ��    �ܣ�ͨ�ſڳ�ʼ������
* ��ڲ�����
* ���ڲ�����
* ��    ����    1�����Ҵ����豸
*               2���޸��豸����
*               3�����ýӿڻص�����  uart_input
********************************************************************************
*/

void uart_app_init(void){
     
    /* �����豸��� */

    
    char uart_name[RT_NAME_MAX];
    
    struct serial_configure config3 = RT_SERIAL_CONFIG_DEFAULT;  /* ��ʼ�����ò��� */
    
    char str[] = "uart3 init success!\r\n";
    rt_strncpy(uart_name, COMM_UART_NAME, RT_NAME_MAX);
    
    /* step1�����Ҵ����豸 */
    serial3 = rt_device_find(uart_name);
    if (!serial3)
    {
        rt_kprintf("find %s failed!\n", uart_name);
    }
    /* step2���޸Ĵ������ò��� */
    config3.baud_rate = BAUD_RATE_9600;        //�޸Ĳ�����Ϊ 9600
    config3.data_bits = DATA_BITS_8;           //����λ 8
    config3.stop_bits = STOP_BITS_1;           //ֹͣλ 1
    config3.bufsz     = 128;                   //�޸Ļ����� buff size Ϊ 128
    config3.parity    = PARITY_NONE;           //����żУ��λ
    
    /* step3�����ƴ����豸��ͨ�����ƽӿڴ�����������֣�����Ʋ��� */
    rt_device_control(serial3, RT_DEVICE_CTRL_CONFIG, &config3);
    
    /* �� DMA ���ռ���ѯ���ͷ�ʽ�򿪴����豸 */
    rt_device_open(serial3, RT_DEVICE_FLAG_DMA_RX);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(serial3, uart_input);
        /* �����ַ��� */
    rt_device_write(serial3, 0, str, (sizeof(str) - 1));

}


void send_com(uint8_t *data, rt_size_t length)
{    struct tx_msg msg;

    msg.data_ptr = data;  /* ָ����Ӧ�����ݿ��ַ */
    msg.data_size = length; /* ���ݿ�ĳ��� */

    /* ���������Ϣָ��� mq ��Ϣ���� */

    rt_mq_send(&ctxmq, (void*)&msg, sizeof(msg));
}







/*�����ӿں�������*/


/*
********************************************************************************
* 
* ��    �ܣ������ڽ������ݻص�������������ɷ��ͽ������ݵ���Ϣ����mrxmq
* ��ڲ�����rt_device_t dev             �����豸����
*           rt_size_t size              ���ݳ���
* ���ڲ�����rt_err_t result
* ��    ����
*           ����ֵ == 0�������ɹ������������ʧ��
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
        /* ��Ϣ������ */
        rt_kprintf("mrxmq message queue full��\n");
    }
    return result;
}
/*
********************************************************************************
* ��    �ܣ������ڳ�ʼ������
* ��ڲ�����
* ���ڲ�����
* ��    ����    1�����Ҵ����豸
*               2���޸��豸����
*               3�����ýӿڻص�����  uart_input
********************************************************************************
*/

void rs485_init(void){
    
    char uart_name[RT_NAME_MAX];
    
    struct serial_configure config2 = RT_SERIAL_CONFIG_DEFAULT;  /* ��ʼ�����ò��� */
    
    char str[] = "uart2 init success!\r\n";
    
    rt_strncpy(uart_name, MEAS_UART_NAME, RT_NAME_MAX);

    rt_pin_mode(RS485_RE, PIN_MODE_OUTPUT);
    
    /* step1�����Ҵ����豸 */
    serial2 = rt_device_find(uart_name);
    if (!serial2)
    {
        rt_kprintf("find %s failed!\n", uart_name);
    }
    /* step2���޸Ĵ������ò��� */
    config2.baud_rate = BAUD_RATE_9600;        //�޸Ĳ�����Ϊ 9600
    config2.data_bits = DATA_BITS_8;           //����λ 8
    config2.stop_bits = STOP_BITS_1;           //ֹͣλ 1
    config2.bufsz     = 128;                   //�޸Ļ����� buff size Ϊ 128
    config2.parity    = PARITY_NONE;           //����żУ��λ
    
    /* step3�����ƴ����豸��ͨ�����ƽӿڴ�����������֣�����Ʋ��� */
    rt_device_control(serial2, RT_DEVICE_CTRL_CONFIG, &config2);
    
    /* �� DMA ���ռ���ѯ���ͷ�ʽ�򿪴����豸 */
    rt_device_open(serial2, RT_DEVICE_FLAG_DMA_RX);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(serial2, rs485_input);
        /* �����ַ��� */
    RS485_REEN(1); 
    rt_device_write(serial2, 0, str, (sizeof(str) - 1));
    RS485_REEN(0); 

}

void send_mea(void *data, rt_size_t length)
{    struct tx_msg msg;

    msg.data_ptr = data;  /* ָ����Ӧ�����ݿ��ַ */
    msg.data_size = length; /* ���ݿ�ĳ��� */

    /* ���������Ϣָ��� mq ��Ϣ���� */
    rt_mq_send(&mtxmq, (void*)&msg, sizeof(msg));
}




