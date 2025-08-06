
#include <rtdevice.h>
#include "task.h"
#include "uart_app.h"
#include "dht11.h"
#include "general.h"

/**< 每一个任务，对应task.c中的一个函数 */
/**< work_index 索引表相关的任务 */
/**< work_command 命令值表相关的任务 */
/**< work_runpara 运行参数相关的任务 */
/**< work_status 状态相关任务 */
struct rt_work  work_index,\
                work_command,\
                work_runpara,\
                work_status,\
                work_aim;


/**
 **********************************************************************
 * @brief      命令解码线程
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
        /* 从消息队列中读取消息*/
        result = rt_mq_recv(&crxmq, &crxmsg, sizeof(crxmsg), RT_WAITING_FOREVER);
      
        if (result == 0)
            {
                /* 从串口读取数据*/
                rx_length = rt_device_read(crxmsg.dev, 0, crx_buffer, crxmsg.size);
                if(verifyModbusCRC(crx_buffer,rx_length ,0)){                                   /**< 如果CRC校验通过，则执行解包程序 */
                    if (crx_buffer[0] == 0x01){                                                 /**< 此处0x01,默认为本机的从站编码 */
                        DecodedModbusPacket decodedPacket = decodeModbusPacket(crx_buffer, rx_length);          /**< 解码数据包 */
                        static DecodedModbusBody decodedBody;
                        decodeModbusBodyCommand(&decodedBody, decodedPacket);                                   /**< 解码命令包体 */
                        task_sort(&decodedBody,&tkmsg);                                                         /**< 生成任务类型 */
                        result = rt_mq_send(&taskmq, &tkmsg, sizeof(tkmsg));
                        if ( result == -RT_EFULL)
                            {
                                /* 消息队列满 */
                                rt_kprintf("message queue full！\n");
                            }
                    }
                    else{
                        send_com(crx_buffer, rx_length);                    /**< 如果不是自己的路由，则转发出去 */
                    }

//                    if(decodedBody.function == 0x10){                                         //如果类型是10，则把写键值数据库任务发送到任务队列
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
 * @brief      通信口发送线程
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
        /* 从消息队列中读取消息*/
        
        result = rt_mq_recv(&ctxmq, &com_tx_msg, sizeof(com_tx_msg), RT_WAITING_FOREVER);
        
        if (result == 0)
        {
            rt_device_write(serial3, 0, com_tx_msg.data_ptr, com_tx_msg.data_size);
            /* 打印数据 */
            rt_kprintf("%s\n",com_tx_msg.data_ptr);
        }
    }
}
/**
 **********************************************************************
 * @brief      测量数据解码线程
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
        /* 从消息队列中读取消息*/
        result = rt_mq_recv(&mrxmq, &mrxmsg, sizeof(mrxmsg), RT_WAITING_FOREVER);
        if (result == 0)
            {
                 /* 从串口读取数据*/
                rx_length = rt_device_read(mrxmsg.dev, 0, mrx_buffer, mrxmsg.size);
                /**< 如果返回的功能码是0x05或0x03,则把接收到的数据发送到 任务命令消息队列 */
                if ((mrx_buffer[1] == 0x05)|(mrx_buffer[1] == 0x03))
                    {
                        result = rt_mq_send(&taskcmdmq, &mrx_buffer, rx_length);
                        if ( result == -RT_EFULL)
                            {
                            rt_kprintf("taskcmdmq queue full！\n");
                            }
                    }
                /**< 如果返回的功能码是0x10或0x06,则把接收到的数据发送到 任务目标值消息队 */
                if((mrx_buffer[1] == 0x10)|(mrx_buffer[1] == 0x06))
                    {
                        result = rt_mq_send(&taskaimmq, &mrx_buffer, rx_length);
                        if ( result == -RT_EFULL)
                            {
                            rt_kprintf("taskaimmq queue full！\n");
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
 * @brief      测量命令发送线程
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
        /* 从消息队列中读取消息*/
        
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
 * @brief      任务分发线程
 *      -索引任务
 *          -#功能码04,            MB_START_ADDR         < 起始地址 < MB_DATA_END_ADDR
 *          -#功能码03或0x10,      MB_PARA_START_ADDR    < 起始地址 < MB_PARA_END_ADDR
 *      -命令值任务
 *          -#功能码05或0x0F,      MB_START_ADDR         < 起始地址 < MB_COIL_END_ADDR
 *      -运行参数读写任务
 *          -#功能码03或0x10，     MB_START_ADDR         < 起始地址 < MB_RUNPARA_END_ADDR
 *      -状态读取任务
 *          -#功能码0x02,          MB_START_ADDR         < 起始地址 < MB_INPUT_END_ADDR
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
//        /* 从消息队列中读取消息*/
//        
//        result = rt_mq_recv(&taskmq, &taskmsg1, sizeof(taskmsg1), RT_WAITING_FOREVER);
//        
//        if (result == 0)
//        {
//            if ((taskmsg1.body.function == 0x04)&&(taskmsg1.body.startingAddress < MB_DATA_END_ADDR ))      /**< 功能码04,MB_START_ADDR< 起始地址 < MB_DATA_END_ADDR 时*/
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
 * @brief      任务分发线程
 *      -索引任务
 *          -#任务类型 READ_PARA，READ_DATA，WRITE_PARA
 *      -命令值任务
 *          -#任务类型 WRITE_CMD
 *      -运行参数任务
 *          -#任务类型  WRITE_RUNPARA
 *      -状态任务
 *          -#任务类型  READ_STATUS
 *      -目标值任务
 *          -#任务类型  WRITE_AIM
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
        /* 从消息队列中读取消息*/
        
        result = rt_mq_recv(&taskmq, &taskmsg1, sizeof(taskmsg1), RT_WAITING_FOREVER);
        
        if (result == 0)
        {
            if ((taskmsg1.cmdtype == READ_PARA)|(taskmsg1.cmdtype == READ_DATA )|(taskmsg1.cmdtype == WRITE_PARA ))      /**< 任务类型为 读参数 或 读数据 或 写参数 时，把索引相关任务推送进工作队列 */
            {
                task_msg database_msg = taskmsg1;
                rt_work_init(&work_index,task_index,&database_msg);
                rt_work_submit(&work_index, 0); 
            }
            else if(taskmsg1.cmdtype == WRITE_RUNPARA)                              /**< 任务类型为 WRITE_RUNPARA */
                {
                    task_msg runpara_msg = taskmsg1;
                    rt_work_init(&work_runpara,task_status,&runpara_msg);           /**< 初始化任务，把任务函数task_status及函数的参数 runpara_msg 放入work_runpara*/
                    rt_work_submit(&work_runpara, 0);                               /**< 提交work_runpara，进入系统工作队列 */
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

