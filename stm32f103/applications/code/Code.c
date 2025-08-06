/******************************************************************************

Copyright (C), 2010-2020,��������ˮ���ؼ����������ι�˾
All rights reserved.

File name:      Code.c
Author:         Warrior
Version:        1.0
Date:           12/23/2010

Description:    ���벢ִ��PCָ��봫�����Ľ�����
                            �����������ĸ���������ǲ�����Ĭ��ID��
                            ��׼��������ID�ǹ̶�ֵ(StdSenID)��

Function List:��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
                1. .........
                2. .........

History:        ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
              1.Mender:Warrior
                Date:12/31/2010
                Version:1.5
                Modify:�������ɹ�����֮������ݸ��½�����ԭʼֵ��
                             ��������Ԥ��ģʽ��ʱ������ڹ۲���ʩ��ִ�У�

Other:        ����������صĺ�������

******************************************************************************/

#include "decode.h"


INT8U TypeCmd=0;
INT16U DummyId_Mdl = 0xFFFF;
BOARD_VAR BoardVar;
DEV_VAR DevVar[MAX_DEVS];
METER_VAR MeterVar[MAX_METER];
SEN_VAR SenVar[MAX_SENS];

const BOARD_PARAM BoardPara = 				// measure board para
{
	BOARD_DEFAULT_ID,				//ID
	MAX_INT16U,							//Belong
	OBJ_BOARD,							//����
	BOARD_MEAS,							//С�࣭������
	0,                      //Role
    FIXT_MEAS,
	15, 									//all
	5,                     //value
	GAP_MINUTE,             //unit
	1,											//��ǰ�ϵ�ʱ��
	PROTOCOL_MODULE,        //USART0 ����Э��
	PROTOCOL_BOARD,         //USART1 ����Э�飬��������PC
	0,											//gap
	1,											//response
    0,                      //Number of station. 20160706 1 = �˲�
    3,                      //Number of board.
	1,                      //Power controled.
	1,                      //0x81
	0,											// �Ƿ����Ӳ���Ԥ�����������1=����
	5,											// �ȴ���������ʱ������
	9600,											//1200/9600
	8*60,										//Loop measure base
	10,											//loop
	40,
	20,
	10,
	CREATE_TIME,						// ���򴴽�ʱ��
	RLS_VERSION,						// ��˾�����汾
	APP_VERSION,						// ����汾

};
/******************************************************************************
Function:     ScanID()
Description:  ɨ������ID����ȡ������༰�����
Input:        id��
Return:       ID��������
Global Var:
Other:        �����Ƿ���ڸö���
History:      ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
              1.Mender:
                Date:
                Version:
                Modify:
*****************************************************************************/
INT8U ScanID(SCAN_OBJ *ps)
{
    INT8U i, num=0;

    if(BoardPara.ID == ps->id)
    {
        num++;
        ps->type = OBJ_BOARD;
    }
    for(i=0; i<MAX_DEVS; i++)
    {
        if(ps->id == DevVar[i].ID)
        {
            num++;
            ps->type = OBJ_DEVICE;
            ps->sn = i;
        }
    }
    for(i=0; i<MAX_METER; i++)
    {
        if(ps->id == MeterVar[i].ID)
        {
            num++;
            ps->type = OBJ_METER;
            ps->sn = i;
        }
    }
    for(i=0; i<MAX_SENS; i++)
    {
        if(ps->id == SenVar[i].ID)
        {
            num++;
            ps->type = OBJ_SENSOR;
            ps->sn = i;
        }
    }


    // ��ʱ�������/����ģ�飬ʹ������
    if(ps->id == DummyId_Mdl){
        ps->type = OBJ_APP_BRD;
        ps->sn = 0;
        num = 1;
    }

    return num;
}

/******************************************************************************
Function:     SelMeasDevice()
Description:  ѡ��һ���۲���ʩ��
Input:
Return:       ������룻
Global Var:
Other:
History:      ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
              1.Mender:
                Date:
                Version:
                Modify:
*****************************************************************************/
INT8U SelMeasDevice(INT16U id, SCAN_OBJ scan)
{
    INT8U s;
    s = ERR_NO_ERR ;
    

    return s;
}


/******************************************************************************
Function:     AddHandle()
Description:  ע��һ������
Input:        �����ţ�����PC�����ݰ�ָ�룻
Return:       ��
Global Var:   ���������۲���ʩ�������洢�ȣ�
Other:        ������ͬ��Ӧ��
              ���id��ɨ�����ִ��������ݽ��Ӧ��
History:      ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
              1.Mender:
                Date:
                Version:
                Modify:
*****************************************************************************/
INT8U AddHandle(INT16U id, INT8U *pmsg)
{
    INT8U s = ERR_NO_ERR ;
    if(ERR_NO_ERR == s)
    {
        *(pmsg + PKG_Len) = 5;
                *(pmsg+PKG_IDH) = (INT8U)(id>>8);
                *(pmsg+PKG_IDL) = (INT8U)(id);
        *(pmsg + PKG_Type) = DAT_RESULT;
        *(pmsg + PKG_Prop) = SUC_ADD_OBJ;
    }
    return s;
}

/******************************************************************************
Function:     DelHandle()
Description:  ע��һ������
Input:        �����ţ�����PC�����ݰ�ָ�룻
Return:       ��
Global Var:   ���������۲���ʩ�������洢�ȣ�
Other:        ������ͬ��Ӧ��
              ���id��ɨ�����ִ��������ݽ��Ӧ��
History:      ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
              1.Mender:
                Date:
                Version:
                Modify:
*****************************************************************************/
INT8U DelHandle(INT16U id, INT8U *pmsg)
{
    INT8U s = ERR_NO_ERR ;
  if(ERR_NO_ERR == s)
  {
    *(pmsg + PKG_Len) = 5;
    *(pmsg+PKG_IDH) = (INT8U)(id>>8);
    *(pmsg+PKG_IDL) = (INT8U)(id);
    *(pmsg + PKG_Type) = DAT_RESULT;
    *(pmsg + PKG_Prop) = SUC_DEL_OBJ;
  }
    return s;
}

/******************************************************************************
Function:      SetHandle()
Description:  ����һ������
Input:        �����ţ�����PC�����ݰ�ָ�룻
Return:       ��
Global Var:   ���������۲���ʩ�������洢�ȣ�
Other:        ������ͬ��Ӧ��
              ���id��ɨ�����ִ��������ݽ��Ӧ��
History:      ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
              1.Mender:
                Date:
                Version:
                Modify:
*****************************************************************************/
INT8U SetHandle(INT16U id, INT8U *pmsg)
{
    INT8U s = ERR_NO_ERR ;
    if(ERR_NO_ERR == s){
        *(pmsg + PKG_Len) = 5;
                *(pmsg+PKG_IDH) = (INT8U)(id>>8);
                *(pmsg+PKG_IDL) = (INT8U)(id);
        *(pmsg + PKG_Type) = DAT_RESULT;
        *(pmsg + PKG_Prop) = SUC_SET_PROP;
    }

    return s;
}



/******************************************************************************
Function:      GetHandle()
Description:  ��ѯһ�����������
Input:        �����ţ�����PC�����ݰ�ָ�룻
Return:       ��
Global Var:   ���������۲���ʩ�������洢�ȣ�
Other:        ������ͬ��Ӧ��
              ���id��ɨ�����ִ��������ݽ��Ӧ��
History:      ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
              1.Mender:
                Date:
                Version:
                Modify:
*****************************************************************************/
INT8U GetHandle(INT16U id, INT8U *pmsg)
{
    INT8U s = ERR_NO_ERR ;
    if(ERR_NO_ERR == s){
        *(pmsg + PKG_Len) = 5;
                *(pmsg+PKG_IDH) = (INT8U)(id>>8);
                *(pmsg+PKG_IDL) = (INT8U)(id);
        *(pmsg + PKG_Type) = DAT_RESULT;
        *(pmsg + PKG_Prop) = SUC_SET_PROP;
    }
    return s;    
        
}

INT8U ResetHandle(INT16U id, INT8U *pmsg)
{
    __set_PRIMASK(1); //close all interrupt
    HAL_NVIC_SystemReset(); //reset
    return ERR_NO_ERR;
}


INT8U ySelMeasHandle(INT16U id, INT8U *pmsg)
{

    return ERR_NO_ERR;
}




INT8U SelMeasHandle(INT16U id, INT8U *pmsg)
{

    return ERR_NO_ERR;
}



/******************************************************************************
Function:      QueryObjHandle()
Description:  ��ѯ����
Input:        �����ţ�����PC�����ݰ�ָ�룻
Return:       ��
Global Var:   ���������۲���ʩ�������洢�ȣ�
Other:        ������ͬ��Ӧ��
              ���id��ɨ�����ִ��������ݽ��Ӧ��
History:      ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
              1.Mender:
                Date:
                Version:
                Modify:
*****************************************************************************/
INT8U QueryObjHandle(INT16U id, INT8U *pmsg)
{
  
  return ERR_NO_ERR;
}



/******************************************************************************
Function:     RTUDecode()
Description:  ���롢ִ������PC�����
Input:        ����PC�����ݰ�ָ�룻
Return:       ��
Global Var:   ���������۲���ʩ�������洢�ȣ�
Other:        ������ͬ��Ӧ��
              ���id��ɨ�����ִ��������ݽ��Ӧ��
History:      ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
              1.Mender:
                Date:
                Version:
                Modify:
*****************************************************************************/

static const DECODE_HANDLE DecodeIndex[] = {
    {CMD_ADD,           DAT_RESULT,         AddHandle},
    {CMD_DEL,           DAT_RESULT,         DelHandle},
    {CMD_SET,           DAT_RESULT,         SetHandle},
    {CMD_GET,           DAT_PARA,           GetHandle},
    {CMD_SEL_MEAS,      DAT_SEL_MEAS,       SelMeasHandle},    //id�ǲ�������ѡ��������ж���id����������ѡ�����ݣ�
    {CMD_QUERY_OBJ,     DAT_QUERY_OBJ,      QueryObjHandle},
    {CMD_QUERY_SEN,     DAT_QUERY_SEN,      QueryObjHandle},

    {CMD_RESET,         DAT_RESULT,         ResetHandle}

  //{CMD_SEL_MEAS_ALL,  DAT_SEL_MEAS, SelMeasAllHandle},
    //{CMD_SEN_SET,     CMD_SET,                SenSetHandle},
    //{CMD_SEN_GET,     CMD_GET,                SenGetHandle},

    //{CMD_SETID,            DAT_RESULT,            CmdSetIDHandle},
    //{CMD_FLASH_WR_T,        DAT_WR_FLASH_T,    CmdRWFlsHandle_t},
    //{CMD_FLASH_RD_T,        DAT_RD_FLASH_T,    CmdRWFlsHandle_t},
};

INT8U DecodeRTU(INT8U *pmsg){
    //INT8U i, cmd, err, num;
    INT8U err, num;
    INT16U id;

  num =  *(pmsg + PKG_InfoDstL);
  num &= 0x3F;  //BoardNum

//  if((BoardVar.numBrd == num) || (num == 0x3F)){
    if((BoardVar.numBrd == num) || (num == 0x01)){
    err = ERR_INVALID_CMD;
    //GetTime(PubTimeStamp);

//    cmd = *(pmsg + PKG_Type);
    id = (INT16U)*(pmsg + PKG_IDH)<<8;
    id += (INT16U)*(pmsg + PKG_IDL);

    num = *(pmsg + PKG_InfoSrcH);
    *(pmsg + PKG_InfoSrcH) = *(pmsg + PKG_InfoDstH);
    *(pmsg + PKG_InfoDstH) = num;

    num = *(pmsg + PKG_InfoSrcL);
    *(pmsg + PKG_InfoSrcL) = *(pmsg + PKG_InfoDstL);
    *(pmsg + PKG_InfoDstL) = num;
        err = ERR_NO_ERR;
    return err;
//    if(MAX_INT16U == id){
//      err = ERR_INVALID_ID;
//    }
//    else{
//      num = sizeof(DecodeIndex)/sizeof(DecodeIndex[0]);
//      err = ERR_INVALID_CMD;
//      //GetTime(PubTimeStamp);

//      for(i=0; i<num; i++){
//        if(DecodeIndex[i].CmdCode == cmd){
//          TypeCmd = *(pmsg + PKG_Type);
//          *(pmsg + PKG_Type) = DecodeIndex[i].BackCode;
//          err = (DecodeIndex[i].handle)(id, pmsg);
//          i = num;
//        }
//      }
//    }
    }

    else
    {
    err = ERR_INVALID_ID;
    }
    return err;
}


/******************************************************************************
Function:     DecodeMODBUS()
Description:  ���롢ִ������LCU���������
Input:        ����LCU�����ݰ�ָ�룻
Return:       ��
Global Var:
Other:        ������ͬ��Ӧ��
              ����ַ��ɨ�����ִ��������ݽ��Ӧ��
History:      ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
              1.Mender:
                Date:
                Version:
                Modify:
*****************************************************************************/
INT8U DecodeModbus(INT8U *pmsg)
{
    /*******************************03������************************************
    �ӻ���ַ  ������  �Ĵ�����ַH  �Ĵ�����ַL �Ĵ�������H  �Ĵ�������L CRCH  CRCL

    �ӻ���ַ  ������  �����ֽ���  DATA1H  DATA1L  DATA2H  DATA2L...CRCH  CRCL
    ***************************************************************************/
   return 0;
}
/*********************************EOF*****************************************/
/*
clear
EB 95 04 10 00 01 00 00 00 00 00 32 00 FF FE 00 00 00 D9 31
Query
EB 95 04 10 00 01 00 00 00 00 00 0a 00 FF FE 00 00 00 D9 31
add
EB 95 0b 10 00 01 00 00 00 00 00 31 00 03 00 01 07 02 08 10 00 03 00 00 00 D9 31
EB 95 0b 10 00 01 00 00 00 00 00 31 00 07 00 01 0b 02 08 10 00 70 00 00 00 D9 31
time
EB 95 05 10 00 01 00 00 00 00 00 02 00 FF FE 2b 00 00 00 D9 31
EB 95 0b 10 00 01 00 00 00 00 00 01 00 FF FE 2b 11 08 09 10 30 00 00 00 00 D9 31

select measuring
EB 95 04 10 00 01 00 00 00 00 00 33 00 07 00 00 00 00 D9 31

*/





