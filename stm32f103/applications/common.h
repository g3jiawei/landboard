

#ifndef __COMMON_H__
#define __COMMON_H__

#include "stdint.h"

/*************************************���峣��*********************************************/
#ifndef NUL
#define NUL             0
#endif

#ifndef TRUE
#define TRUE                 1
#endif

#ifndef FALSE
#define FALSE                 0
#endif

#ifndef ON
#define ON                    1
#endif

#ifndef OFF
#define OFF                    0
#endif

//#ifndef SUCCESS
//#define SUCCESS                1
//#endif

#ifndef FAILURE
#define FAILURE                0
#endif

//#ifndef ENABLE
//#define ENABLE                1
//#endif

//#ifndef DISABLE
//#define DISABLE                0
//#endif

#ifndef NULL
#define NULL          ((void *)0)
#endif

#define MAX_FLOAT   (3.402823E38)
#define MIN_FLOAT   (-3.402823E38)

enum DT{
    DT_INT8 =0,                   //
    DT_UINT8,
    DT_INT16,                   //��������Ϊ16λ����
    DT_INT32,                   //��������Ϊ32λ����
    DT_UINT16,                  //��������Ϊ16λ�޷�������
    DT_UINT32,                  //��������Ϊ32λ�޷�������
    DT_FT32,                    //�������͵����ȸ�����
    DT_FT64,                    //˫���ȸ�����
};
/***************************�������ݴ洢��ظ�ʽ*******************************************/

#define     MAX_SEN         16

/*������ѹ�ƴ��������ݸ�ʽ*/
typedef struct{
    float frequency;
    float temperature;
}MSR_FRE;

/*������贫�������ݸ�ʽ*/    
typedef struct{
    float R;
    float R_ratio;
}MSR_RESISTANCE;

/*�����������������ݸ�ʽ*/
typedef struct{
    float value;
    int code;
}MSR_OTHER;

/*���廷�������ݸ�ʽ*/
typedef struct{
    float temperature;
    float humidity;
    float voltage;
}MSR_ENVIRONMENT;





/****************��������洢��ظ�ʽ******************************************************/

/*���������������ݸ�ʽ*/
typedef struct{
    uint16_t    addr;
    uint8_t     data_type;
    uint8_t     data_len;
}PHYSICS;

/*����ͨ�����ñ����ݸ�ʽ*/
typedef struct{
    uint16_t    addr;
    uint8_t     role;
    uint16_t    obj_num;
}TUN_CONFIG;

/*���������������ϵ�����ݸ�ʽ*/
typedef struct{
    uint16_t    addr;
    uint8_t     para_code;
    uint8_t     phy_code;
}PHY_REL_PARA;

/*���������������Ŀ�����ݸ�ʽ*/
typedef struct{
    uint16_t    addr;
    uint16_t    monitor_code;
    uint8_t     phy_code;
}PHY_MONITOR;

/*������������ݸ�ʽ*/
typedef struct{
    uint16_t    addr;
    uint8_t     role;
    uint8_t     para_val;
}PARA;






#endif

















