

#ifndef __COMMON_H__
#define __COMMON_H__

#include "stdint.h"

/*************************************定义常量*********************************************/
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
    DT_INT16,                   //数据类型为16位整形
    DT_INT32,                   //数据类型为32位整形
    DT_UINT16,                  //数据类型为16位无符号整形
    DT_UINT32,                  //数据类型为32位无符号整形
    DT_FT32,                    //数据类型单精度浮点数
    DT_FT64,                    //双精度浮点数
};
/***************************定义数据存储相关格式*******************************************/

#define     MAX_SEN         16

/*定义渗压计传感器数据格式*/
typedef struct{
    float frequency;
    float temperature;
}MSR_FRE;

/*定义差阻传感器数据格式*/    
typedef struct{
    float R;
    float R_ratio;
}MSR_RESISTANCE;

/*定义其它传感器数据格式*/
typedef struct{
    float value;
    int code;
}MSR_OTHER;

/*定义环境量数据格式*/
typedef struct{
    float temperature;
    float humidity;
    float voltage;
}MSR_ENVIRONMENT;





/****************定义参数存储相关格式******************************************************/

/*定义物理量表数据格式*/
typedef struct{
    uint16_t    addr;
    uint8_t     data_type;
    uint8_t     data_len;
}PHYSICS;

/*定义通道配置表数据格式*/
typedef struct{
    uint16_t    addr;
    uint8_t     role;
    uint16_t    obj_num;
}TUN_CONFIG;

/*定义参数物理量关系表数据格式*/
typedef struct{
    uint16_t    addr;
    uint8_t     para_code;
    uint8_t     phy_code;
}PHY_REL_PARA;

/*定义物理量监测项目表数据格式*/
typedef struct{
    uint16_t    addr;
    uint16_t    monitor_code;
    uint8_t     phy_code;
}PHY_MONITOR;

/*定义参数表数据格式*/
typedef struct{
    uint16_t    addr;
    uint8_t     role;
    uint8_t     para_val;
}PARA;






#endif

















