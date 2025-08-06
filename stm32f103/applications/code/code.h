/******************************************************************************

Copyright (C), 2010-2020,西安兰特水电测控技术有限责任公司
All rights reserved.

File name:        volar.h
Author:         Warrior
Version:        1.0
Date:                    11/02/2010

Description:    说明主要功能，与其他模块或函数的接口，输出值，取值范围，
                含义及参数间的控制、顺序、独立或依赖等关系；

Other:          与协议编解码相关的处理函数声明及常量定义

Function List:主要函数列表，每条记录应包含函数名及功能简要说明
                1. .........
                2. .........

History:        每条修改历史记录应包括修改者、日期、版本及修改内容简述
                1.Mender:
                  Date:
                  Modify:
            


******************************************************************************/

#ifndef _CODE_H_
#define _CODE_H_

typedef unsigned char        BOOL;                //布尔变量(字节)
typedef unsigned int        BOOLEAN;        //布尔变量(字)
typedef unsigned char        INT8U;            //无符号8位整型变量
typedef signed char            INT8S;            //有符号8位整型变量
typedef unsigned short    INT16U;            //无符号16位整型变量
typedef signed short        INT16S;            //有符号16位整型变量
typedef unsigned long        INT32U;            //无符号32位整型变量
typedef signed long            INT32S;            //有符号32位整型变量
typedef unsigned int        INTU;                //系统相关无符号整型变量
typedef signed int            INTS;                //系统相关有符号整型变量
typedef float                        FP32;                //单精度浮点数（32位长度）
typedef double                    FP64;                //双精度浮点数（64位长度）

typedef struct cmd_handle
{
    INT8U CmdCode;                    //命令编码
    INT8U BackCode;                    //返回码
    INT8U (*handle)(INT16U id, INT8U* pmsg);
}DECODE_HANDLE;

typedef struct ScanObj
{
    INT16U id;
    INT8U type;
    INT8U sn;
}SCAN_OBJ;
/*
********************************************************************************
* 函数名称：RTUDecode()
* 功    能：从串口接收数据包的包体中提取参数到数组 pPara 中
* 入口参数：无
* 出口参数：无
* 描    述：
* 全局变量：
********************************************************************************
*/
//void RTUDecode(INT8U *pCmd);
INT8U DecodeRTU(INT8U *pd);
INT8U DecodeModbus(INT8U *pmsg);

#endif

/*********************************EOF*****************************************/

/****************************
添加
eb 96 0b 31 00 00 20 01 07 02 01 03 00 02 00 00

删除
eb 96 04 32 00 00 20 00 00

设置
eb 96 06 01 00 00 20 16 02 00 00

查询
eb 96 06 02 00 00 20 16 17 00 00
eb 96 06 02 00 ff fe 0C 11 00 00

选测
eb 96 04 33 00 00 20 00 00

查询对象
eb 96 04 0a 00 00 20 00 00

查询数据
eb 96 04 08 00 00 20 00 00






*********************************/







