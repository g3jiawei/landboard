/******************************************************************************

Copyright (C), 2010-2020,��������ˮ���ؼ����������ι�˾
All rights reserved.

File name:        volar.h
Author:         Warrior
Version:        1.0
Date:                    11/02/2010

Description:    ˵����Ҫ���ܣ�������ģ������Ľӿڣ����ֵ��ȡֵ��Χ��
                ���弰������Ŀ��ơ�˳�򡢶����������ȹ�ϵ��

Other:          ��Э��������صĴ�������������������

Function List:��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
                1. .........
                2. .........

History:        ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
                1.Mender:
                  Date:
                  Modify:
            


******************************************************************************/

#ifndef _CODE_H_
#define _CODE_H_

typedef unsigned char        BOOL;                //��������(�ֽ�)
typedef unsigned int        BOOLEAN;        //��������(��)
typedef unsigned char        INT8U;            //�޷���8λ���ͱ���
typedef signed char            INT8S;            //�з���8λ���ͱ���
typedef unsigned short    INT16U;            //�޷���16λ���ͱ���
typedef signed short        INT16S;            //�з���16λ���ͱ���
typedef unsigned long        INT32U;            //�޷���32λ���ͱ���
typedef signed long            INT32S;            //�з���32λ���ͱ���
typedef unsigned int        INTU;                //ϵͳ����޷������ͱ���
typedef signed int            INTS;                //ϵͳ����з������ͱ���
typedef float                        FP32;                //�����ȸ�������32λ���ȣ�
typedef double                    FP64;                //˫���ȸ�������64λ���ȣ�

typedef struct cmd_handle
{
    INT8U CmdCode;                    //�������
    INT8U BackCode;                    //������
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
* �������ƣ�RTUDecode()
* ��    �ܣ��Ӵ��ڽ������ݰ��İ�������ȡ���������� pPara ��
* ��ڲ�������
* ���ڲ�������
* ��    ����
* ȫ�ֱ�����
********************************************************************************
*/
//void RTUDecode(INT8U *pCmd);
INT8U DecodeRTU(INT8U *pd);
INT8U DecodeModbus(INT8U *pmsg);

#endif

/*********************************EOF*****************************************/

/****************************
���
eb 96 0b 31 00 00 20 01 07 02 01 03 00 02 00 00

ɾ��
eb 96 04 32 00 00 20 00 00

����
eb 96 06 01 00 00 20 16 02 00 00

��ѯ
eb 96 06 02 00 00 20 16 17 00 00
eb 96 06 02 00 ff fe 0C 11 00 00

ѡ��
eb 96 04 33 00 00 20 00 00

��ѯ����
eb 96 04 0a 00 00 20 00 00

��ѯ����
eb 96 04 08 00 00 20 00 00






*********************************/







