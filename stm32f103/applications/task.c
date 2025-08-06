/**
 ***********************************************************************************************
 * @file          task.c
 * @author        ʯ����
 * @version       v1.0
 * @date          20240719
 * @brief         ���е�������
 * @license       Copyright (c) 2024-2048, ��������ˮ���ؼ����������ι�˾
 **********************************************************************************************
 * @attention 
 * 
 * Modified Records
 * 
 ***********************************************************************************************
*/



#include "task.h"
#include "dht11.h"
#include "rtc.h"
#include "database.h"
#include "general.h"

/**
 **********************************************************************
 * @brief      ������������
 * @param      struct rt_work *work ��������
 * @param      void *workdata �������
 * @retval     
 **********************************************************************
 */

void task_index(struct rt_work *work,void *workdata){
    
    uint8_t txdata[64];
    rt_size_t txlength = 0;
    rwdatabase data1;
    task_msg_t data = (task_msg_t )workdata;
    data1.addr = data->body.startingAddress;
    data1.length = data->body.byteCount;
    data1.data = data->body.data;

    storeToDatabase( &data1);              //�洢���������ݿ�
    
    txlength = encodeModbusData(txdata, (DecodedModbusBody *)workdata);     //���ɷ�����Ϣ

    send_com(txdata,  txlength);             //���ͷ�����Ϣ�����Ͷ���
}
/**
 **********************************************************************
 * @brief      �����������
 * @param      struct rt_work *work ��������
 * @param      void *workdata �������
 * @retval     
 **********************************************************************
 */
void task_command(struct rt_work *work,void *workdata){
    uint8_t txdata[64];
    rt_size_t txlength = 0;
    rwdatabase data1;
    DecodedModbusBody *data = (DecodedModbusBody *)workdata;
    data1.addr = data->startingAddress;
    
    if ((retrieveFromDatabase(&data1)) == 0){
    
        data->byteCount = data1.length;
        data->data = data1.data ;
        txlength =  encodeModbusData(txdata,(DecodedModbusBody *)workdata);
        
    }
        send_com(txdata,txlength );
}

/**
 **********************************************************************
 * @brief      ���в�����������
 * @param      struct rt_work *work ��������
 * @param      void *workdata �������
 * @retval     
 **********************************************************************
 */
void task_runpara(struct rt_work *work,void *workdata)
{

}

/**
 **********************************************************************
 * @brief      ״̬��������
 * @param      struct rt_work *work ��������
 * @param      void *workdata �������
 * @retval     
 **********************************************************************
 */
void task_status(struct rt_work *work,void *workdata)
{

}

/**
 **********************************************************************
 * @brief      Ŀ��ֵ������
 * @param      struct rt_work *work ��������
 * @param      void *workdata �������
 * @retval     
 **********************************************************************
 */
void task_aim(struct rt_work *work,void *workdata)
{

}

/**
 **********************************************************************
 * @brief      ������������
 * @param      struct rt_work *work ��������
 * @param      void *workdata �������
 * @retval     
 **********************************************************************
 */

void task_sort(DecodedModbusBody *body,task_msg_t msg){
//    task_msg_t msg = (task_msg_t)workdata;
    if (body->function == 04)
    {
        if ((body->startingAddress < MB_DATA_END_ADDR )&&(body->startingAddress > MB_START_ADDR ))
            msg->cmdtype = READ_DATA;
    }
    else if(body->function == 03)
  {
        if ((body->startingAddress < MB_PARA_END_ADDR )&&(body->startingAddress > MB_PARA_START_ADDR))
            msg->cmdtype = READ_PARA;
    }
    else if(body->function == 10)
    {
        if ((body->startingAddress < MB_PARA_END_ADDR )&&(body->startingAddress > MB_PARA_START_ADDR))
            msg->cmdtype = WRITE_PARA;
        else if((body->startingAddress < MB_RUNPARA_END_ADDR )&&(body->startingAddress > MB_START_ADDR))
            msg->cmdtype = WRITE_RUNPARA;
            else if ((body->startingAddress < MB_AIM_END_ADDR )&&(body->startingAddress > MB_START_ADDR))
                msg->cmdtype = WRITE_AIM;
    }
    else if(body->function == 05)
    {
        if ((body->startingAddress < MB_COIL_END_ADDR )&&(body->startingAddress > MB_START_ADDR))
            msg->cmdtype = WRITE_CMD;
    }
    else if(body->function == 02)
    {
        if ((body->startingAddress < MB_INPUT_END_ADDR )&&(body->startingAddress > MB_START_ADDR))
            msg->cmdtype = READ_STATUS;
    }
    msg->body = *body;

}
