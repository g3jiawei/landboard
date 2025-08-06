
#include <rtdevice.h>
#include "task.h"
#include "uart_app.h"
#include "dht11.h"
#include "general.h"

/**< ÿһ�����񣬶�Ӧtask.c�е�һ������ */
/**< work_index ��������ص����� */
/**< work_command ����ֵ����ص����� */
/**< work_runpara ���в�����ص����� */
/**< work_status ״̬������� */
struct rt_work  work_index,\
                work_command,\
                work_runpara,\
                work_status,\
                work_aim;


/**
 **********************************************************************
 * @brief      ��������߳�
 * @param      
 * @param      
 * @retval     
 **********************************************************************
 */

void decodecom_thread_entry(void *para)
{
    struct rx_msg crxmsg;
    task_msg tkmsg;
    rt_err_t result;
    rt_uint8_t rx_length;
    


    while (1)
   {
        rt_memset(&crxmsg, 0, sizeof(crxmsg));
        /* ����Ϣ�����ж�ȡ��Ϣ*/
        result = rt_mq_recv(&crxmq, &crxmsg, sizeof(crxmsg), RT_WAITING_FOREVER);
      
        if (result == 0)
            {
                /* �Ӵ��ڶ�ȡ����*/
                rx_length = rt_device_read(crxmsg.dev, 0, crx_buffer, crxmsg.size);
                if(verifyModbusCRC(crx_buffer,rx_length ,0)){                                   /**< ���CRCУ��ͨ������ִ�н������ */
                    if (crx_buffer[0] == 0x01){                                                 /**< �˴�0x01,Ĭ��Ϊ�����Ĵ�վ���� */
                        DecodedModbusPacket decodedPacket = decodeModbusPacket(crx_buffer, rx_length);          /**< �������ݰ� */
                        static DecodedModbusBody decodedBody;
                        decodeModbusBodyCommand(&decodedBody, decodedPacket);                                   /**< ����������� */
                        task_sort(&decodedBody,&tkmsg);                                                         /**< ������������ */
                        result = rt_mq_send(&taskmq, &tkmsg, sizeof(tkmsg));
                        if ( result == -RT_EFULL)
                            {
                                /* ��Ϣ������ */
                                rt_kprintf("message queue full��\n");
                            }
                    }
                    else{
                        send_com(crx_buffer, rx_length);                    /**< ��������Լ���·�ɣ���ת����ȥ */
                    }

//                    if(decodedBody.function == 0x10){                                         //���������10�����д��ֵ���ݿ������͵��������
//                        rt_work_init(&write_kvdb,kvdb_write,&decodedBody);
//                        rt_work_submit(&write_kvdb, 0);  
//                        
//                    }else if(decodedBody.function == 0x03){
//                        rt_work_init(&read_kvdb,kvdb_read,&decodedBody);
//                        rt_work_submit(&read_kvdb, 0);
//                    
//                    }

                }

             }
    }
}

/**
 **********************************************************************
 * @brief      ͨ�ſڷ����߳�
 * @param      
 * @param      
 * @retval     
 **********************************************************************
 */
void comsend_thread_entry(void *para )
{
    struct tx_msg com_tx_msg;
    rt_err_t result;


    while (1)
    {
        rt_memset(&com_tx_msg, 0, sizeof(com_tx_msg));
        /* ����Ϣ�����ж�ȡ��Ϣ*/
        
        result = rt_mq_recv(&ctxmq, &com_tx_msg, sizeof(com_tx_msg), RT_WAITING_FOREVER);
        
        if (result == 0)
        {
            rt_device_write(serial3, 0, com_tx_msg.data_ptr, com_tx_msg.data_size);
            /* ��ӡ���� */
            rt_kprintf("%s\n",com_tx_msg.data_ptr);
        }
    }
}
/**
 **********************************************************************
 * @brief      �������ݽ����߳�
 * @param      
 * @param      
 * @retval     
 **********************************************************************
 */
void decodemea_thread_entry(void *para)
{
    struct rx_msg mrxmsg;
    rt_err_t result;
    rt_uint32_t rx_length;
//    KeyValuePair testKeyVal[2];
//    uint16_t regSn=41234;
 //   uint8_t i, num;
    while (1)
   {
        rt_memset(&mrxmsg, 0, sizeof(mrxmsg));
        /* ����Ϣ�����ж�ȡ��Ϣ*/
        result = rt_mq_recv(&mrxmq, &mrxmsg, sizeof(mrxmsg), RT_WAITING_FOREVER);
        if (result == 0)
            {
                 /* �Ӵ��ڶ�ȡ����*/
                rx_length = rt_device_read(mrxmsg.dev, 0, mrx_buffer, mrxmsg.size);
                /**< ������صĹ�������0x05��0x03,��ѽ��յ������ݷ��͵� ����������Ϣ���� */
                if ((mrx_buffer[1] == 0x05)|(mrx_buffer[1] == 0x03))
                    {
                        result = rt_mq_send(&taskcmdmq, &mrx_buffer, rx_length);
                        if ( result == -RT_EFULL)
                            {
                            rt_kprintf("taskcmdmq queue full��\n");
                            }
                    }
                /**< ������صĹ�������0x10��0x06,��ѽ��յ������ݷ��͵� ����Ŀ��ֵ��Ϣ�� */
                if((mrx_buffer[1] == 0x10)|(mrx_buffer[1] == 0x06))
                    {
                        result = rt_mq_send(&taskaimmq, &mrx_buffer, rx_length);
                        if ( result == -RT_EFULL)
                            {
                            rt_kprintf("taskaimmq queue full��\n");
                            }
                    }
//                DecodedModbusPacket decodedPacket = decodeModbusPacket(mrx_buffer,rx_length );
//                DecodedModbusBody decodedBody;        //
//                decodeModbusBodyData(&decodedBody, decodedPacket, regSn);
//               num = extractRegisterPairs(testKeyVal, decodedBody.data, decodedBody.count, regSn, 4);
//                for(i=0; i<num; i++){
//                    fdb_kv_set_blob(&kvdb, (testKeyVal+i)->keyChar, fdb_blob_make(&blob, (testKeyVal+i)->val, 4));
//                }
//                free(decodedBody.data);

            }
    }
}

/**
 **********************************************************************
 * @brief      ����������߳�
 * @param      
 * @param      
 * @retval     
 **********************************************************************
 */
void measend_thread_entry(void *para )
{
    struct tx_msg mea_tx_msg;
    rt_err_t result;
    rt_uint8_t dev_id;

    while (1)
    {
        rt_memset(&mea_tx_msg, 0, sizeof(mea_tx_msg));
        /* ����Ϣ�����ж�ȡ��Ϣ*/
        
        result = rt_mq_recv(&mtxmq, &mea_tx_msg, sizeof(mea_tx_msg), RT_WAITING_FOREVER);
        dev_id = *(mea_tx_msg.data_ptr);
        if (result == 0)
        {
            RS485_REEN(1);
            rt_device_write(serial2, 0, mea_tx_msg.data_ptr, mea_tx_msg.data_size);
            RS485_REEN(0);
        }
    }
}

/**
 **********************************************************************
 * @brief      ����ַ��߳�
 *      -��������
 *          -#������04,            MB_START_ADDR         < ��ʼ��ַ < MB_DATA_END_ADDR
 *          -#������03��0x10,      MB_PARA_START_ADDR    < ��ʼ��ַ < MB_PARA_END_ADDR
 *      -����ֵ����
 *          -#������05��0x0F,      MB_START_ADDR         < ��ʼ��ַ < MB_COIL_END_ADDR
 *      -���в�����д����
 *          -#������03��0x10��     MB_START_ADDR         < ��ʼ��ַ < MB_RUNPARA_END_ADDR
 *      -״̬��ȡ����
 *          -#������0x02,          MB_START_ADDR         < ��ʼ��ַ < MB_INPUT_END_ADDR
 * @param      
 * @param      
 * @retval     
 **********************************************************************
 */
//void taskmgt_thread_entry(void *para )
//{
//    task_msg taskmsg1;
//    rt_err_t result;


//    while (1)
//    {
//        rt_memset(&taskmsg1, 0, sizeof(taskmsg1));
//        /* ����Ϣ�����ж�ȡ��Ϣ*/
//        
//        result = rt_mq_recv(&taskmq, &taskmsg1, sizeof(taskmsg1), RT_WAITING_FOREVER);
//        
//        if (result == 0)
//        {
//            if ((taskmsg1.body.function == 0x04)&&(taskmsg1.body.startingAddress < MB_DATA_END_ADDR ))      /**< ������04,MB_START_ADDR< ��ʼ��ַ < MB_DATA_END_ADDR ʱ*/
//            {
//                task_msg data_msg;
//                data_msg = taskmsg1;
//                rt_work_init(&work_index,task_index,&data_msg);
//                rt_work_submit(&work_index, 0); 
//            }
//            else if(((taskmsg1.body.function == 0x03)|(taskmsg1.body.function == 0x10)))
//                {
//                    if(taskmsg1.body.startingAddress <MB_RUNPARA_END_ADDR )
//                    {
//                        task_msg runpara_msg;
//                        runpara_msg = taskmsg1;
//                        rt_work_init(&work_runpara,task_status,&runpara_msg);
//                        rt_work_submit(&work_runpara, 0);
//                    }
//                    else if(taskmsg1.body.startingAddress <MB_PARA_END_ADDR)
//                        {
//                            task_msg index_msg;
//                            index_msg = taskmsg1;
//                            rt_work_init(&work_index,task_index,&index_msg);
//                            rt_work_submit(&work_index, 0);
//                        }                    
//                }
//                else if ((taskmsg1.body.function == 0x02)&&(taskmsg1.body.startingAddress <MB_INPUT_END_ADDR ))
//                    {
//                        task_msg status_msg;
//                        status_msg = taskmsg1;
//                        rt_work_init(&work_status,task_status,&status_msg);
//                        rt_work_submit(&work_status, 0);
//                    }
//                else if (((taskmsg1.body.function == 0x05)|(taskmsg1.body.function == 0x0F))&&(taskmsg1.body.startingAddress <MB_COIL_END_ADDR))
//                    {
//                        task_msg command_msg;
//                        command_msg = taskmsg1;
//                        rt_work_init(&work_command,task_status,&command_msg);
//                        rt_work_submit(&work_command, 0);
//                    }
//            }
//        }
//}

/**
 **********************************************************************
 * @brief      ����ַ��߳�
 *      -��������
 *          -#�������� READ_PARA��READ_DATA��WRITE_PARA
 *      -����ֵ����
 *          -#�������� WRITE_CMD
 *      -���в�������
 *          -#��������  WRITE_RUNPARA
 *      -״̬����
 *          -#��������  READ_STATUS
 *      -Ŀ��ֵ����
 *          -#��������  WRITE_AIM
 * @param      
 * @param      
 * @retval     
 **********************************************************************
 */

void taskmgt_thread_entry(void *para )
{
    task_msg taskmsg1;
    rt_err_t result;


    while (1)
    {
        rt_memset(&taskmsg1, 0, sizeof(taskmsg1));
        /* ����Ϣ�����ж�ȡ��Ϣ*/
        
        result = rt_mq_recv(&taskmq, &taskmsg1, sizeof(taskmsg1), RT_WAITING_FOREVER);
        
        if (result == 0)
        {
            if ((taskmsg1.cmdtype == READ_PARA)|(taskmsg1.cmdtype == READ_DATA )|(taskmsg1.cmdtype == WRITE_PARA ))      /**< ��������Ϊ ������ �� ������ �� д���� ʱ������������������ͽ��������� */
            {
                task_msg database_msg = taskmsg1;
                rt_work_init(&work_index,task_index,&database_msg);
                rt_work_submit(&work_index, 0); 
            }
            else if(taskmsg1.cmdtype == WRITE_RUNPARA)                              /**< ��������Ϊ WRITE_RUNPARA */
                {
                    task_msg runpara_msg = taskmsg1;
                    rt_work_init(&work_runpara,task_status,&runpara_msg);           /**< ��ʼ�����񣬰�������task_status�������Ĳ��� runpara_msg ����work_runpara*/
                    rt_work_submit(&work_runpara, 0);                               /**< �ύwork_runpara������ϵͳ�������� */
                 }
            else if(taskmsg1.cmdtype == WRITE_AIM)
                {
                    task_msg aim_msg = taskmsg1;
                    rt_work_init(&work_aim,task_index,&aim_msg);
                    rt_work_submit(&work_index, 0);
                 }                    
            else if(taskmsg1.cmdtype == READ_STATUS)
                {
                    task_msg status_msg = taskmsg1;
                    rt_work_init(&work_status,task_status,&status_msg);
                    rt_work_submit(&work_status, 0);
                }
            else if (taskmsg1.cmdtype == WRITE_CMD)
                {
                    task_msg command_msg = taskmsg1;
                    rt_work_init(&work_command,task_command,&command_msg);
                    rt_work_submit(&work_command, 0);
                }
        }
    }
}

/**
 **********************************************************************
 * @brief      
 * @param      
 * @param      
 * @retval     
 **********************************************************************
 */
void meamode_thread_entry(void *para)
{

}

/**
 **********************************************************************
 * @brief      
 * @param      
 * @param      
 * @retval     
 **********************************************************************
 */
void measure_thread_entry(void *para)
{

}

