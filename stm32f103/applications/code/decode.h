/******************************************************************************

Copyright (C), 2010-2020,��������ˮ���ؼ����������ι�˾
All rights reserved.

File name:        decode.h
Author:         neale
Version:        1.0
Date:            04/01/2024

Description:    �����ͷ�ļ�

Other:

Function List:    ��������Ҫ�Ľṹ�������
                1. .........
                2. .........

History:        ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
                1.Mender:
                  Date:
                  Modify:



******************************************************************************/

#ifndef _DCODE_H_
#define _DCODE_H_

#include "code.h"
#include "stm32f1xx_hal.h"
//#include <uart_app.h>


//������device.h

// �۲���ʩ����
typedef struct DeviceVariable
{
    INT16U    ID;
    INT16U    ReptCnt;    //�ϱ����ݺ�����
    INT16U    MeasCnt;    //�����Լ��Ĳ���ʱ�������
    INT16U    runTimes;                        // ������ʱ��        ����ʱ��
    INT16U    dlyTimes;                        // �����ȴ�ʱ��    ��������
    INT16U    measTimes;                        // ȡ������
    INT8U        Action;
    INT8U        Sign;       //���
     INT8U        Skewing;    //�����Ա��е�ƫ��
     INT8U        StartIn;    // ����ģʽ    �ֶ�/�Զ�=0
     INT8U        StartOut;// ������ʽ    ���/����=0
     INT8U        DevAlt;
}DEV_VAR;


extern  DEV_VAR DevVar[];   //Ҫ���ϵ�ʱ����ʼ����430ʹ�� __no_init ʵ��


//������meter.h

//�߼�����������
typedef struct LogicSensorVariable
{
    FP32 val;
    INT16U    ID;
    INT16U    Belong;

    INT16U    coff1;    // ����
    INT16U    coff2;    // �ܲ���

    INT8U     Sign;
    INT8U     StartIn;
    INT8U        StartOut;
    // INT8U        ErrCode;
    // INT8U        RelayFlag;    //�༶���Ӳ�����еĽ�����־
    INT8U        SeqNum;            //��λ��
    INT8U     Skewing;        //�����Ա��е�ƫ����
    INT8U        alt1;
    INT8U        alt2;
    INT8U        dummy;
}METER_VAR;

//������sensor.h


//����������
typedef struct RealSensorVariable
{
    FP32 val;
    INT16U        Rtlid;             // ȡ������
    INT16U        RegSn;            // �Ĵ�����ַ
    
    INT16U    ID;
    INT16U    Belong;
    INT8U     Sign;
    INT8U     StartIn;
    INT8U        StartOut;
    //INT8U     ErrCode;
    
    INT8U     Skewing;
    //INT8U        SeqNum;
    INT8U     ChnlNum;    //��ͨ��������ʱ��ͨ����
    INT8U     ModuleId;    //����ģ����
    INT8U     InsideId;    //����ģ���ڲ��ı��
    INT8U     subFamily;
    INT8U     subGenus;
    INT8U     dummy;
    INT8U     mark;
    INT8U     status;    // �� �� ��(����ʧ��) ....  ����(�Ĵ�������)
}SEN_VAR;

//������apibasic.h
extern METER_VAR MeterVar[];
extern SEN_VAR SenVar[];




//������cosntdef.h

#define PROTOCOL_BOARD      0   //EB 95 2��SN
#define PROTOCOL_MODULE     1   //EB 96

#define HEAD_SN_LEN         2

#if(2 == HEAD_SN_LEN)
#define HEAD_LEN                11        // RTU2.0 ��ͷ���ֳ���
#else
#define HEAD_LEN                10        // RTU2.0 ��ͷ���ֳ���

#endif
#define TAIL_LEN                3            // RTU2.0 ��β���ֳ���
#define CRC16_LEN                2            // RTU2.0 ��βУ�鲿�ֳ���


#define MAX_DEVS            4        //Ŀǰ�涨Ϊ16����������ֵ��������������ͬ�����£�
#define MAX_METER        32        //�߼�������
#define MAX_SENS            128        //�涨ͨ����0ͨ���ӱ�׼�����������ʵ�ﴫ��������

enum ERROR_VALUE
{
    ERR_NO_ERR = 0,                              // 0x0, û�д���

    ERR_RESET = 1,                              // ��λ
    ERR_INVALID_CMD,                            // ����ʶ���ָ��
    ERR_INVALID_ID,                              // ��Ч ID(0xffff)
    ERR_INVALID_OBJ,                            // ��Ч�Ķ���
    ERR_INVALID_PROP,                            // ��Ч������

  ERR_INVALID_TYPE,             // ��Ч������(�����С���������Ч)
  ERR_INVALID_FATHER,           // �����󲻴���
  ERR_MAX_OBJS,                 // ����û�пռ���Ӷ����� 
  ERR_OBJ_EXISTED,              // ��Ӧ ID �Ķ����Ѿ����� 
  ERR_OBJ_UNFOUND,              // δ������Ӧ ID �Ķ���

  ERR_SYS_BUSY = 11,            //ϵͳ��ʱ�޷�Ӧ��
  //ERR_INVALID_TYPE,             // ��Ч������
    //ERR_INIT_CAL,                                //��ʼ�����㲿���쳣

    //ERR_BOARD_MODE,                            // �����Ӧ��ģʽ�ٽ��д˲���
    //ERR_MEAS_MODE,                           // �����Ӧ��ģʽ�ٽ��д˲���
  SUC_ADD_OBJ = 0x11,           // ��Ӷ���ɹ� 
  SUC_DEL_OBJ,                  // ɾ������ɹ� 
  SUC_SET_PROP,                 // ���ò����ɹ� 
  SUC_QRY_SUBOBJ,               // ��ѯ�Ӷ���ɹ� 
  SUC_CLR_DAT,                  // ������ݳɹ� 
  SUC_QRY_DAT_BY_SN,            // ��ѯ���ݰ��ɹ� 
  SUC_QRY_NEWEST,               // ��ѯ�������ݳɹ� 


  
    //0x20~0x6f
    ERR_SEN_FAIL = 0x21,                  //����������ʧ��
    //ERR_SEN_OVER_RANGE,                    //�������������
    //ERR_SEN_POS,                                //��������λ����
    ERR_SEN_CONNECT,                          //���������Ӵ���
    //ERR_SEN_RET,                                //���������ش���
    //ERR_SEN_NO_INIT = 0x26,            //��Ӵ�����δ���ó�ֵ
    ERR_SEN_OUT_OF_RANGE,                  //��������
  ERR_SEN_DIGITAL,              //���ִ���������
    //ERR_SEN_DISTURB,                        //����������
    //ERR_SEN_PARAM,                            //��������������
    //ERR_SEN_STD_ERR,                        //��׼����������
    //ERR_METER_NUM,                             //�۲���ʩ����������
    //ERR_METER_OVER_RANGE,                //�߼�����������������

  
    //ERR_DEV_NOSEN = 0x31,                // �۲���ʩû�д�����
    //ERR_DEV_TYPE,                                // �۲���ʩ����������岻��
    //ERR_DEV_SENS,                                // �۲���ʩ�Ĵ�������������
    //ERR_DEV_SQE,                             // �۲���ʩ�Ĵ�������Ŵ���
    //ERR_DEV_RANGE,                             //�۲���ʩ�����̴���
  //ERR_SEN_NUM,                //(�۲���ʩ����������)����ģʽ�����˶��������
    //ERR_DEV_NUM,                             //�۲���ʩ����������
    
    ERR_CHB_LINK_FAIL = 0x31,        //ͨ��������ʧ��
    ERR_CHB_SWITCH_FAIL,                //ͨ�����л�ʧ��

    ERR_CTRL_OPT_FAIL,                // ��������Ӧ��

    ERR_SEN_INIT = 0x35,                        // ���޳�ʼ����...
    ERR_SEN_INVARIANT,                            // ��ֵ����
    ERR_SEN_INSTABILITY,                        // ��ֵ���ȶ�
    ERR_SEN_TOO_MANY,                                // �������̫��
  ERR_SEN_VAL_NULL,                  // ��������Ϊ��

    ERR_SEN_INIT_K,                             // �������ݳ�ʼ��
    ERR_SEN_OVER_1,                             // �����������̫��
    ERR_SEN_OVER_2,                             // �����������̫��
    
    ERR_MODULE_LINK_FAIL = 0x49,//ģ������ʧ��
    ERR_MODULE_MEAS_FAIL,                //ģ�����ʧ��

  ERR_TRACE_MODE = 0x60,        //����ģʽ�����˶����������۲���ʩ
    ERR_DEV_NO_BASE,                            //���û�׼���ɹ�
  ERR_INIT_CAL,                 //��ʼ�����㲿���쳣
    ERR_INIT_PARAM,                      //��ʼ����������
  ERR_DATA_IMPERFECT,           //���ݲ�����
  ERR_DATA_LIMITED,             //���ݳ���(�������ּ������)
  //ERR_DATA_OVER = 0xf0,                //���ݳ���
  ERR_BUSY,                              //�ڴ��쳣

    ERR_NO_RET_I = 0xf1,                //���ط�����Ϣ(�ڲ�����)
    /*    
    SUC_ADD_OBJ = 0x51,                 // 0x81, ��Ӷ���ɹ�
    SUC_DEL_OBJ = 0x52,                 // 0x02, ɾ������ɹ�
    SUC_QRY_DAT = 0x54,                 // 0x03, ��ѯ���ݳɹ�
    SUC_QRY_OBJ = 0x55,                 // 0x04, ��ѯ����ɹ�
    SUC_SET_PROP = 0x56,                // 0x05, ���ò����ɹ�
    */
    ERR_Q_FULL_I,                                // ������
    ERR_UPDATA_DATUM,                     //�۲������´���
    ERR_OTHER_ERR
};


// �������ݰ����ͣ��г��������Ӧ��Ĺؼ��֣�
enum PKG_CMD_TYPE
{
    CMD_SET = 1,                        // 0x01, ���ö�������
    CMD_GET,                                // 0x02, ��ѯ��������
    
    //CMD_STATE_QUERY,                // 0x03, ��ѯ�弶����״̬
    //CMD_SETID,                            // 0x04, ���ð弶������
    //CMD_STATE_MEAS,                    // 0x05, ѡ��弶����״̬
    CMD_RESET = 6,                    // 0x06, ��λ�弶����
    CMD_QUERY_DAT = 8,            // 0x08, ���ݲ�ѯ

  CMD_QUERY_OBJ = 0x0a,   //�����ѯ
  CMD_QUERY_SEN = 0x0b,   //�����ѯ,Ϊ������ǰ�ĵ��Թ���

    CMD_SEN_SET = 0x11,            //��������ʽ���������ϵĸ������
    CMD_SEN_GET = 0x12,         //��ѯ����ʽ���������ϵĸ������

    
  //CMD_QUERY_DEV = 0x0a,    // 0x0a, ��ʩ��ѯ
    //CMD_QUERY_SEN = 0x0b,    // 0x0b, ��ʩ������������ѯ
    CMD_RTU_WAKE = 0x1E,        // 0x1E, �������֪������ϵ�

    CMD_MEAS_I = 48,                // 0x30, ����
    CMD_ADD = 49,                        // 0x31, ��Ӷ���
    CMD_DEL = 50,                        // 0x32, ɾ������
    CMD_SEL_MEAS = 51,            // 0x33, ѡ��
    CMD_CH_SEL_I = 52,            // 0x34, ͨ��ѡ��
    CMD_CLR_DATUM = 53,            //0x35, �������

  CMD_SEL_MEAS_ALL = 54,  // 0x36, ѡ������������д�����������
    CMD_GET_MEAS_ALL = 3,        // 0x37, ȡ�����������й۲���ʩ�Ľ��
    
    CMD_SET_OPENING = 0x60, // 0x60�����ÿ���
    CMD_CTRL_UNIT_OPT = 0x61,    // 0x60������������
    
    CMD_PROG_START = 80,        // 0x50, ��� P
    CMD_ERASE = 69,                    // 0x45, ���� E
    CMD_PROG_DATA = 68,            // 0x44, ���� D
    CMD_PROG_ADDR = 64,            // 0x40, ��ַ @
    CMD_PROG_END = 81,            // 0x51, ���� Q

    CMD_FLASH_WR_T = 0x61,    // 0x61, д AT45
    CMD_FLASH_RD_T = 0x62,    // 0x62, �� AT45
  CMD_SEL_MEAS_D = 0x63,  // 0x63��ѡ�����ִ�����
  CMD_GET_PARA_B = 0x64,  // 0x64����ȡ������������


    DAT_DAT = 0x81,                    // ��������
    DAT_STATE = 0x82,                // ״̬����
    DAT_PARA = 0x83,                // ��������
    DAT_RESULT = 0x84,            // �����ɹ�����Ӧ����ӡ�ɾ���ȵķ���
    DAT_EXCEPT = 0x85,            // �쳣��Ϣ
    DAT_QUERY_DAT = 0x86,        // ���ݲ�ѯ�ķ���
//    DAT_CH_TEST = 0x87,        // �ŵ�����
    DAT_SEL_MEAS = 0x88,        // ѡ������
    DAT_STATE_QUERY = 0x89,    // ѡ��״̬

  DAT_QUERY_OBJ = 0x8a,   //�����ѯ
  //DAT_QUERY_DEV = 0x8a,    // ��ʩ��ѯ
    DAT_QUERY_SEN = 0x8b,    // ��ʩ������������ѯ

  DAT_DAT_T = 0x91,                // ��������

  //DAT_DAT_END = 0x92,                //�۲���ʩ���ݷ�����ϱ�־

  DAT_DAT_END = 201,                //�۲���ʩ���ݷ�����ϱ�־

    CMD_POWER_ON = 202,              //����ģ���ϵ�����
  DAT_POWER_ON = 203,                //����ģ���ϵ�ɹ�


    DAT_WR_FLASH_T = 0xE1,    // 0xE1, Write to flash �ɹ�
    DAT_RD_FLASH_T = 0xE2,    // 0xE2, Data read from flash

    PKG_TYPE_OTHER
};

enum RTU_PACKAGE
{
    PKG_EB  = 0,
    PKG_95  = 1,
    PKG_Len = 2,
    PKG_InfoDstH,
    PKG_InfoDstL,
    PKG_InfoSrcH,
    PKG_InfoSrcL,
    PKG_RouteH,
    PKG_RouteL,
    PKG_SN1,
    PKG_SN2,
    /*
    */
    #if(2 == HEAD_SN_LEN)
    PKG_Type = 11,
    #else
    PKG_Type = 10,
    #endif
    PKG_ProjCode,
    PKG_IDH,
    PKG_IDL,
    PKG_Prop,
    PKG_Value,
    PKG_Prop_Time = PKG_Prop,
    PKG_Value_Time,
    PKG_Prop_1st = PKG_Prop_Time + 7,
    PKG_Value_1st,
    PKG_Prop_2nd = PKG_Prop_1st + 5,
    PKG_Value_2nd,
    PKG_Prop_3rd = PKG_Prop_2nd + 5,
    PKG_Value_3rd,
    //PKG_CorrectH = HEAD_LEN + BODY_LEN,
    PKG_CorrectL,
    PKG_OTHER
};

// RTU2.0 for sensor ��
enum RTU4S_PACKAGE
{
    PKG4S_EB,
    PKG4S_90,
    PKG4S_Len,
    PKG4S_CMD,
    PKG4S_PROJ,
    PKG4S_ADDR,
    PKG4S_CHNL,
    PKG4S_Prop,
    PKG4S_Value0,
    PKG4S_Value1,
    PKG4S_Value2,
    PKG4S_Value3,
};


enum OBJECT_CLASS
{
    OBJ_CENTER = 1,        // 01, ����վ
    OBJ_CARD,                    // 02, �ӿڿ�
    OBJ_REMOTE,                // 03, ң���ն�
    OBJ_RELAY,                // 04, �м�վ
    OBJ_MB,                        // 05, ������
    //OBJ_PORT,                    // 06, �˿�
    // OBJ_METER = 6,        //�߼�������
    // OBJ_SENSOR = 7,     //ʵ�ﴫ����
    // OBJ_DEVICE,             // 08, �۲���ʩ
    OBJ_DEVICE = 0x19,        // LCU
    OBJ_METER = 0x1b,        // ģ���ϵ
    OBJ_SENSOR = 0x1c,     // ���ݹ�ϵ
    OBJ_MONITOR = 9,            // 09, ������
    OBJ_BOARD,                // 0A, �弶����

    OBJ_DEST = 0x0E,                // 0E, �ɴ�����
    //OBJ_SENSOR,                 // , ʵ�ﴫ����
    OBJ_APP_BRD = 0x10,    // Ӧ���࣬ģ��/������

    OBJ_OBS = 0x7F,        //observe variable
};

// �弶��������
enum BOARD_OBJECT_CLASS{
    BOARD_COMM = 1,    // ͨѶ���ư�
    BOARD_MEAS            // ������
};

//���������ģʽ
enum BOARD_MEAS_MODE{
    LOOP_MEAS = 1,        //Ѳ��
    PRE_MEAS = 2,            //Ԥ��
    FIXT_MEAS = 3,        //���⣬Ԥ���һ�֣����������ͬ���ָ���Ϊ����Ӧ��;
    TRACE_MEAS = 4,        //���٣��ָ���Ϊ�����Ա���
  SERIES_MEAS = 5,  //�������������Ƶȣ�
  TRACK_T_MEAS = 6, //�¸���ģʽ
  
  LCU_MEAS = 7, //��LCU���ģʽ���ȴ�ѡ���ȡ����
    OTHER_MEAS
};

enum FIXED_MEASURE_GAP_UNIT
{
  GAP_SECOND = 1,
  GAP_MINUTE = 2,
  GAP_HOUR = 3,
  GAP_DAY = 4,
    
  GAP_OTHER
};

//������common.h
#define MAX_INT16U        0xffff


//������board.h



#define BOARD_DEFAULT_ID        0xFFFE    //������Ĭ��ID

#if (0 == ULTRA_FLOW)
// ͨ�ò�����
#define APP_VERSION        "RTU-2.24$"            //ʹ��Э��
// #define RLS_VERSION        "LCU-1905B1$"        //�̼�����汾
#define RLS_VERSION        "CTRL1912T3$"        //�̼�����汾
#define CREATE_TIME        "12/25/2019$"        //���򴴽�/�޸�ʱ��W
#elif (1 == ULTRA_FLOW)
// ����������
#define APP_VERSION        "RTU-2.24$"            //ʹ��Э��
#define RLS_VERSION        "CLL-181001$"    //�̼�����汾
// #define RLS_VERSION        "ZQL-1903B1$"    //�̼�����汾
// #define RLS_VERSION        "ZHL-1903B1$"    //�̼�����汾
#define CREATE_TIME        "10/10/2018$"        //���򴴽�/�޸�ʱ��W

#elif (2 == ULTRA_FLOW)
// Һ�鳬��������
#define APP_VERSION        "RTU-2.24$"            //ʹ��Э��
// #define RLS_VERSION        "Evap160701$"    //�̼�����汾
// #define CREATE_TIME        "07/04/2016$"        //���򴴽�/�޸�ʱ��W
#define RLS_VERSION        "EVA-181001$"    //�̼�����汾
#define CREATE_TIME        "10/12/2017$"        //���򴴽�/�޸�ʱ��W

#else
#error Use undefined.
#endif


//���������
typedef struct BoardVariable{
    INT16U FixedMeasCnt;    //����ʱ����������Լ�
    INT16U FixedReportCnt;//�����ϱ�ʱ�������������  20130223���������֣�
    INT16U GapInDay;            //�������������
    INT16U MinMeasPrd;            //Ԥ����С���
    INT16U MaxMeasPrd;            //Ԥ�������
    INT16U ReportT;            //�ϱ������
    
    INT8U GapDays;                //��������е�����
    INT8U PastDays;                //Ѳ��ʱ�Ѿ���ȥ������
    INT8U MeasPreTime;        //������ǰʱ�䣬����
    INT8U MeasMode;                //
    
    INT8U DevNum;                    //
    INT8U MeterNum;                //
    INT8U SenNum;                    //
    INT8U MinUnit;                //
    INT8U GapNum;                    //
    INT8U numBrd;                  // ���
    INT8U save;                    // �ϱ�����
    INT8U workMode;                  // ����ģʽ
    INT8U numStation;                //
    INT8U dummy;                    //
    INT8U ctrlSw;                //
    INT8U ctrlDly;                    //
} BOARD_VAR;

//��������� ��������
typedef struct BoardParameter{
    INT16U ID;                //������ID
    INT16U Belong;        //�����ϲ�
    INT8U Class;            //�������
    INT8U SubClass;        //����С��
    
    INT8U    Role;                //��ţ�������ɫ
    INT8U MeasMode;   //������Ĳ���ģʽ
    INT8U SaveItem;        //�ϱ�����ѡ��
    INT8U FixedT;     //����������ֵ
    INT8U MinUnit;    //�������ĵ�λ
    INT8U PrePowerOn;    //��ǰ�ϵ�ʱ��(���ӣ�ֻ�ڽڵ�ģʽ����Ч)
    INT8U P0Protocol; //����0�Ľ���Э��
    INT8U P1Protocol; //����1�Ľ���Э��
    INT8U GapNum;            //���ڿ�������ʾ��������巢�����ݵ�Ƶ�ʣ�
                      //0�������͵���ʾ����n��ÿn�������巢��һ�Σ�
    INT8U ReportSwitch;      //������ʽ��1���Ա���2��Ӧ���������ִ�������

    INT8U NumStation; //����վ��;    2014/02/05 �˲����� 0����ͨ��1��������
    INT8U NumBoard;   //���
    INT8U PwrControl; //��������Դ���ƣ�0�����ӵ磻
    INT8U WorkMode;   //������ʽ���ϱ�����Ϊ:0��0x88��1��0x81��

    INT8U CtrlUnitSw; // �Ƿ���������� 1=��Ҫ����������
    INT8U CtrlUnitDly;   // �������ȴ���ʱ������
    INT16U BaudRate2;            //�����ģ��Ĵ��ڲ�����

    INT16U LoopMeasB;    //Ѳ���׼(Сʱ������)
    INT16U MeasT;            //Ѳ����(Сʱ������)
    INT16U MaxMeasPrd;//����������(Сʱ������)
    INT16U MinMeasPrd;//��С��������(Сʱ������)
    INT16U ReportT;        //�������϶����ϱ����ݵ������ʱ��(Сʱ������)
    INT8U crt_time[12];    //����ʱ��
    INT8U rls_ver[12];        //��˾�����汾
    INT8U app_ver[12];        //����汾
} BOARD_PARAM;

extern const BOARD_PARAM BoardPara;
extern BOARD_VAR BoardVar;

#endif

