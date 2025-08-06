/******************************************************************************

Copyright (C), 2010-2020,西安兰特水电测控技术有限责任公司
All rights reserved.

File name:      Code.c
Author:         Warrior
Version:        1.0
Date:           12/23/2010

Description:    解码并执行PC指令，与传感器的交互；
                            独立传感器的父对象编码是测量板默认ID，
                            标准传感器的ID是固定值(StdSenID)；

Function List:主要函数列表，每条记录应包含函数名及功能简要说明
                1. .........
                2. .........

History:        每条修改历史记录应包括修改者、日期、版本及修改内容简述
              1.Mender:Warrior
                Date:12/31/2010
                Version:1.5
                Modify:传感器成功测量之后的数据更新仅更新原始值；
                             若工作于预测模式，时间更新在观测设施中执行；

Other:        与编码解码相关的函数定义

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
	OBJ_BOARD,							//大类
	BOARD_MEAS,							//小类－测量板
	0,                      //Role
    FIXT_MEAS,
	15, 									//all
	5,                     //value
	GAP_MINUTE,             //unit
	1,											//提前上电时间
	PROTOCOL_MODULE,        //USART0 接收协议
	PROTOCOL_BOARD,         //USART1 接收协议，测量板与PC
	0,											//gap
	1,											//response
    0,                      //Number of station. 20160706 1 = 滤波
    3,                      //Number of board.
	1,                      //Power controled.
	1,                      //0x81
	0,											// 是否连接测量预处理控制器，1=连接
	5,											// 等待控制器延时，分钟
	9600,											//1200/9600
	8*60,										//Loop measure base
	10,											//loop
	40,
	20,
	10,
	CREATE_TIME,						// 程序创建时间
	RLS_VERSION,						// 公司发布版本
	APP_VERSION,						// 程序版本

};
/******************************************************************************
Function:     ScanID()
Description:  扫描现有ID，获取对象大类及其序号
Input:        id；
Return:       ID的数量；
Global Var:
Other:        查找是否存在该对象；
History:      每条修改历史记录应包括修改者、日期、版本及修改内容简述
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


    // 暂时处理测量/控制模块，使不报错；
    if(ps->id == DummyId_Mdl){
        ps->type = OBJ_APP_BRD;
        ps->sn = 0;
        num = 1;
    }

    return num;
}

/******************************************************************************
Function:     SelMeasDevice()
Description:  选测一个观测设施；
Input:
Return:       错误代码；
Global Var:
Other:
History:      每条修改历史记录应包括修改者、日期、版本及修改内容简述
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
Description:  注册一个对象；
Input:        对象编号，来自PC的数据包指针；
Return:       ；
Global Var:   传感器、观测设施变量，存储等；
Other:        基本等同于应答；
              检查id，扫描命令－执行命令－根据结果应答；
History:      每条修改历史记录应包括修改者、日期、版本及修改内容简述
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
Description:  注册一个对象；
Input:        对象编号，来自PC的数据包指针；
Return:       ；
Global Var:   传感器、观测设施变量，存储等；
Other:        基本等同于应答；
              检查id，扫描命令－执行命令－根据结果应答；
History:      每条修改历史记录应包括修改者、日期、版本及修改内容简述
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
Description:  设置一个对象；
Input:        对象编号，来自PC的数据包指针；
Return:       ；
Global Var:   传感器、观测设施变量，存储等；
Other:        基本等同于应答；
              检查id，扫描命令－执行命令－根据结果应答；
History:      每条修改历史记录应包括修改者、日期、版本及修改内容简述
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
Description:  查询一个对象参数；
Input:        对象编号，来自PC的数据包指针；
Return:       ；
Global Var:   传感器、观测设施变量，存储等；
Other:        基本等同于应答；
              检查id，扫描命令－执行命令－根据结果应答；
History:      每条修改历史记录应包括修改者、日期、版本及修改内容简述
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
Description:  查询对象；
Input:        对象编号，来自PC的数据包指针；
Return:       ；
Global Var:   传感器、观测设施变量，存储等；
Other:        基本等同于应答；
              检查id，扫描命令－执行命令－根据结果应答；
History:      每条修改历史记录应包括修改者、日期、版本及修改内容简述
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
Description:  解码、执行来自PC的命令；
Input:        来自PC的数据包指针；
Return:       ；
Global Var:   传感器、观测设施变量，存储等；
Other:        基本等同于应答；
              检查id，扫描命令－执行命令－根据结果应答；
History:      每条修改历史记录应包括修改者、日期、版本及修改内容简述
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
    {CMD_SEL_MEAS,      DAT_SEL_MEAS,       SelMeasHandle},    //id是测量板则选测板上所有对象，id是其他对象选测数据；
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
Description:  解码、执行来自LCU主机的命令；
Input:        来自LCU的数据包指针；
Return:       ；
Global Var:
Other:        基本等同于应答；
              检查地址，扫描命令－执行命令－根据结果应答；
History:      每条修改历史记录应包括修改者、日期、版本及修改内容简述
              1.Mender:
                Date:
                Version:
                Modify:
*****************************************************************************/
INT8U DecodeModbus(INT8U *pmsg)
{
    /*******************************03功能码************************************
    从机地址  功能码  寄存器地址H  寄存器地址L 寄存器数量H  寄存器数量L CRCH  CRCL

    从机地址  功能码  返回字节数  DATA1H  DATA1L  DATA2H  DATA2L...CRCH  CRCL
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





