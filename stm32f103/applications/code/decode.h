/******************************************************************************

Copyright (C), 2010-2020,西安兰特水电测控技术有限责任公司
All rights reserved.

File name:        decode.h
Author:         neale
Version:        1.0
Date:            04/01/2024

Description:    解码的头文件

Other:

Function List:    解码所需要的结构体变量等
                1. .........
                2. .........

History:        每条修改历史记录应包括修改者、日期、版本及修改内容简述
                1.Mender:
                  Date:
                  Modify:



******************************************************************************/

#ifndef _DCODE_H_
#define _DCODE_H_

#include "code.h"
#include "stm32f1xx_hal.h"
//#include <uart_app.h>


//复制与device.h

// 观测设施变量
typedef struct DeviceVariable
{
    INT16U    ID;
    INT16U    ReptCnt;    //上报数据后清零
    INT16U    MeasCnt;    //用于自减的测量时间计数器
    INT16U    runTimes;                        // 泵运行时间        控制时间
    INT16U    dlyTimes;                        // 测量等待时间    控制周期
    INT16U    measTimes;                        // 取数周期
    INT8U        Action;
    INT8U        Sign;       //标记
     INT8U        Skewing;    //在属性表中的偏移
     INT8U        StartIn;    // 控制模式    手动/自动=0
     INT8U        StartOut;// 工作方式    间隔/连续=0
     INT8U        DevAlt;
}DEV_VAR;


extern  DEV_VAR DevVar[];   //要求上电时不初始化，430使用 __no_init 实现


//复制于meter.h

//逻辑传感器变量
typedef struct LogicSensorVariable
{
    FP32 val;
    INT16U    ID;
    INT16U    Belong;

    INT16U    coff1;    // 步长
    INT16U    coff2;    // 总步数

    INT8U     Sign;
    INT8U     StartIn;
    INT8U        StartOut;
    // INT8U        ErrCode;
    // INT8U        RelayFlag;    //多级浮子测距仪中的接力标志
    INT8U        SeqNum;            //方位号
    INT8U     Skewing;        //在属性表中的偏移量
    INT8U        alt1;
    INT8U        alt2;
    INT8U        dummy;
}METER_VAR;

//复制于sensor.h


//传感器变量
typedef struct RealSensorVariable
{
    FP32 val;
    INT16U        Rtlid;             // 取数对象
    INT16U        RegSn;            // 寄存器地址
    
    INT16U    ID;
    INT16U    Belong;
    INT8U     Sign;
    INT8U     StartIn;
    INT8U        StartOut;
    //INT8U     ErrCode;
    
    INT8U     Skewing;
    //INT8U        SeqNum;
    INT8U     ChnlNum;    //与通道板连接时的通道号
    INT8U     ModuleId;    //测量模块编号
    INT8U     InsideId;    //测量模块内部的编号
    INT8U     subFamily;
    INT8U     subGenus;
    INT8U     dummy;
    INT8U     mark;
    INT8U     status;    // 读 报 错(更新失败) ....  字数(寄存器数量)
}SEN_VAR;

//复制于apibasic.h
extern METER_VAR MeterVar[];
extern SEN_VAR SenVar[];




//复制于cosntdef.h

#define PROTOCOL_BOARD      0   //EB 95 2个SN
#define PROTOCOL_MODULE     1   //EB 96

#define HEAD_SN_LEN         2

#if(2 == HEAD_SN_LEN)
#define HEAD_LEN                11        // RTU2.0 包头部分长度
#else
#define HEAD_LEN                10        // RTU2.0 包头部分长度

#endif
#define TAIL_LEN                3            // RTU2.0 包尾部分长度
#define CRC16_LEN                2            // RTU2.0 包尾校验部分长度


#define MAX_DEVS            4        //目前规定为16个，这两个值必须与测量板参数同步更新；
#define MAX_METER        32        //逻辑传感器
#define MAX_SENS            128        //规定通道板0通道接标准传感器；最大实物传感器数量

enum ERROR_VALUE
{
    ERR_NO_ERR = 0,                              // 0x0, 没有错误

    ERR_RESET = 1,                              // 复位
    ERR_INVALID_CMD,                            // 不可识别的指令
    ERR_INVALID_ID,                              // 无效 ID(0xffff)
    ERR_INVALID_OBJ,                            // 无效的对象
    ERR_INVALID_PROP,                            // 无效的属性

  ERR_INVALID_TYPE,             // 无效的类型(大类或小类的类型无效)
  ERR_INVALID_FATHER,           // 父对象不存在
  ERR_MAX_OBJS,                 // 板上没有空间添加对象了 
  ERR_OBJ_EXISTED,              // 相应 ID 的对象已经存在 
  ERR_OBJ_UNFOUND,              // 未发现相应 ID 的对象

  ERR_SYS_BUSY = 11,            //系统暂时无法应答
  //ERR_INVALID_TYPE,             // 无效的类型
    //ERR_INIT_CAL,                                //初始化计算部分异常

    //ERR_BOARD_MODE,                            // 请改至应答模式再进行此操作
    //ERR_MEAS_MODE,                           // 请改至应答模式再进行此操作
  SUC_ADD_OBJ = 0x11,           // 添加对象成功 
  SUC_DEL_OBJ,                  // 删除对象成功 
  SUC_SET_PROP,                 // 设置参数成功 
  SUC_QRY_SUBOBJ,               // 查询子对象成功 
  SUC_CLR_DAT,                  // 清除数据成功 
  SUC_QRY_DAT_BY_SN,            // 查询数据包成功 
  SUC_QRY_NEWEST,               // 查询最新数据成功 


  
    //0x20~0x6f
    ERR_SEN_FAIL = 0x21,                  //传感器测量失败
    //ERR_SEN_OVER_RANGE,                    //传感器输出超限
    //ERR_SEN_POS,                                //传感器方位错误
    ERR_SEN_CONNECT,                          //传感器连接错误
    //ERR_SEN_RET,                                //传感器返回错误
    //ERR_SEN_NO_INIT = 0x26,            //外接传感器未设置初值
    ERR_SEN_OUT_OF_RANGE,                  //读数超限
  ERR_SEN_DIGITAL,              //数字传感器故障
    //ERR_SEN_DISTURB,                        //传感器干扰
    //ERR_SEN_PARAM,                            //传感器参数错误
    //ERR_SEN_STD_ERR,                        //标准传感器错误
    //ERR_METER_NUM,                             //观测设施的数量错误
    //ERR_METER_OVER_RANGE,                //逻辑传感器计算结果超限

  
    //ERR_DEV_NOSEN = 0x31,                // 观测设施没有传感器
    //ERR_DEV_TYPE,                                // 观测设施类型与测量板不符
    //ERR_DEV_SENS,                                // 观测设施的传感器数量不够
    //ERR_DEV_SQE,                             // 观测设施的传感器序号错误
    //ERR_DEV_RANGE,                             //观测设施的量程错误
  //ERR_SEN_NUM,                //(观测设施的数量错误)跟踪模式连接了多个传感器
    //ERR_DEV_NUM,                             //观测设施的数量错误
    
    ERR_CHB_LINK_FAIL = 0x31,        //通道板连接失败
    ERR_CHB_SWITCH_FAIL,                //通道板切换失败

    ERR_CTRL_OPT_FAIL,                // 控制器无应答

    ERR_SEN_INIT = 0x35,                        // 门限初始化中...
    ERR_SEN_INVARIANT,                            // 测值不变
    ERR_SEN_INSTABILITY,                        // 测值不稳定
    ERR_SEN_TOO_MANY,                                // 替代次数太多
  ERR_SEN_VAL_NULL,                  // 计算数据为空

    ERR_SEN_INIT_K,                             // 趋势数据初始化
    ERR_SEN_OVER_1,                             // 门限替代次数太多
    ERR_SEN_OVER_2,                             // 方差替代次数太多
    
    ERR_MODULE_LINK_FAIL = 0x49,//模块连接失败
    ERR_MODULE_MEAS_FAIL,                //模块测量失败

  ERR_TRACE_MODE = 0x60,        //跟踪模式连接了多个传感器或观测设施
    ERR_DEV_NO_BASE,                            //设置基准不成功
  ERR_INIT_CAL,                 //初始化计算部分异常
    ERR_INIT_PARAM,                      //初始化参数错误
  ERR_DATA_IMPERFECT,           //数据不完整
  ERR_DATA_LIMITED,             //数据超限(包括各种计算输出)
  //ERR_DATA_OVER = 0xf0,                //数据超限
  ERR_BUSY,                              //内存异常

    ERR_NO_RET_I = 0xf1,                //不必返回信息(内部错误)
    /*    
    SUC_ADD_OBJ = 0x51,                 // 0x81, 添加对象成功
    SUC_DEL_OBJ = 0x52,                 // 0x02, 删除对象成功
    SUC_QRY_DAT = 0x54,                 // 0x03, 查询数据成功
    SUC_QRY_OBJ = 0x55,                 // 0x04, 查询对象成功
    SUC_SET_PROP = 0x56,                // 0x05, 设置参数成功
    */
    ERR_Q_FULL_I,                                // 队列满
    ERR_UPDATA_DATUM,                     //观测量更新错误
    ERR_OTHER_ERR
};


// 定义数据包类型，列出所有命令、应答的关键字；
enum PKG_CMD_TYPE
{
    CMD_SET = 1,                        // 0x01, 设置对象属性
    CMD_GET,                                // 0x02, 查询对象属性
    
    //CMD_STATE_QUERY,                // 0x03, 查询板级对象状态
    //CMD_SETID,                            // 0x04, 设置板级对象编号
    //CMD_STATE_MEAS,                    // 0x05, 选测板级对象状态
    CMD_RESET = 6,                    // 0x06, 复位板级对象
    CMD_QUERY_DAT = 8,            // 0x08, 数据查询

  CMD_QUERY_OBJ = 0x0a,   //对象查询
  CMD_QUERY_SEN = 0x0b,   //对象查询,为兼容以前的调试工具

    CMD_SEN_SET = 0x11,            //设置数字式传感器板上的各类参数
    CMD_SEN_GET = 0x12,         //查询数字式传感器板上的各类参数

    
  //CMD_QUERY_DEV = 0x0a,    // 0x0a, 设施查询
    //CMD_QUERY_SEN = 0x0b,    // 0x0b, 设施所属传感器查询
    CMD_RTU_WAKE = 0x1E,        // 0x1E, 测量板告知管理板上电

    CMD_MEAS_I = 48,                // 0x30, 测量
    CMD_ADD = 49,                        // 0x31, 添加对象
    CMD_DEL = 50,                        // 0x32, 删除对象
    CMD_SEL_MEAS = 51,            // 0x33, 选测
    CMD_CH_SEL_I = 52,            // 0x34, 通道选择
    CMD_CLR_DATUM = 53,            //0x35, 清除数据

  CMD_SEL_MEAS_ALL = 54,  // 0x36, 选测测量板上所有传感器并计算
    CMD_GET_MEAS_ALL = 3,        // 0x37, 取测量板上所有观测设施的结果
    
    CMD_SET_OPENING = 0x60, // 0x60，设置开度
    CMD_CTRL_UNIT_OPT = 0x61,    // 0x60，控制器操作
    
    CMD_PROG_START = 80,        // 0x50, 编程 P
    CMD_ERASE = 69,                    // 0x45, 擦除 E
    CMD_PROG_DATA = 68,            // 0x44, 数据 D
    CMD_PROG_ADDR = 64,            // 0x40, 地址 @
    CMD_PROG_END = 81,            // 0x51, 结束 Q

    CMD_FLASH_WR_T = 0x61,    // 0x61, 写 AT45
    CMD_FLASH_RD_T = 0x62,    // 0x62, 读 AT45
  CMD_SEL_MEAS_D = 0x63,  // 0x63，选测数字传感器
  CMD_GET_PARA_B = 0x64,  // 0x64，读取测量板对象参数


    DAT_DAT = 0x81,                    // 测量数据
    DAT_STATE = 0x82,                // 状态数据
    DAT_PARA = 0x83,                // 属性数据
    DAT_RESULT = 0x84,            // 操作成功：相应于添加、删除等的返回
    DAT_EXCEPT = 0x85,            // 异常信息
    DAT_QUERY_DAT = 0x86,        // 数据查询的返回
//    DAT_CH_TEST = 0x87,        // 信道测试
    DAT_SEL_MEAS = 0x88,        // 选测数据
    DAT_STATE_QUERY = 0x89,    // 选测状态

  DAT_QUERY_OBJ = 0x8a,   //对象查询
  //DAT_QUERY_DEV = 0x8a,    // 设施查询
    DAT_QUERY_SEN = 0x8b,    // 设施所属传感器查询

  DAT_DAT_T = 0x91,                // 测试数据

  //DAT_DAT_END = 0x92,                //观测设施数据发送完毕标志

  DAT_DAT_END = 201,                //观测设施数据发送完毕标志

    CMD_POWER_ON = 202,              //测量模块上电命令
  DAT_POWER_ON = 203,                //测量模块上电成功


    DAT_WR_FLASH_T = 0xE1,    // 0xE1, Write to flash 成功
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

// RTU2.0 for sensor 包
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
    OBJ_CENTER = 1,        // 01, 中心站
    OBJ_CARD,                    // 02, 接口卡
    OBJ_REMOTE,                // 03, 遥测终端
    OBJ_RELAY,                // 04, 中继站
    OBJ_MB,                        // 05, 测量板
    //OBJ_PORT,                    // 06, 端口
    // OBJ_METER = 6,        //逻辑传感器
    // OBJ_SENSOR = 7,     //实物传感器
    // OBJ_DEVICE,             // 08, 观测设施
    OBJ_DEVICE = 0x19,        // LCU
    OBJ_METER = 0x1b,        // 模块关系
    OBJ_SENSOR = 0x1c,     // 数据关系
    OBJ_MONITOR = 9,            // 09, 监测对象
    OBJ_BOARD,                // 0A, 板级对象

    OBJ_DEST = 0x0E,                // 0E, 可达信宿
    //OBJ_SENSOR,                 // , 实物传感器
    OBJ_APP_BRD = 0x10,    // 应用类，模块/控制器

    OBJ_OBS = 0x7F,        //observe variable
};

// 板级对象类型
enum BOARD_OBJECT_CLASS{
    BOARD_COMM = 1,    // 通讯控制板
    BOARD_MEAS            // 测量板
};

//测量板测量模式
enum BOARD_MEAS_MODE{
    LOOP_MEAS = 1,        //巡测
    PRE_MEAS = 2,            //预测
    FIXT_MEAS = 3,        //定测，预测的一种，测量间隔相同，现更改为定测应答;
    TRACE_MEAS = 4,        //跟踪，现更改为定测自报；
  SERIES_MEAS = 5,  //连续，如流量计等；
  TRACK_T_MEAS = 6, //新跟踪模式
  
  LCU_MEAS = 7, //与LCU配合模式，等待选测和取数；
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

//复制于common.h
#define MAX_INT16U        0xffff


//复制于board.h



#define BOARD_DEFAULT_ID        0xFFFE    //测量板默认ID

#if (0 == ULTRA_FLOW)
// 通用测量板
#define APP_VERSION        "RTU-2.24$"            //使用协议
// #define RLS_VERSION        "LCU-1905B1$"        //固件程序版本
#define RLS_VERSION        "CTRL1912T3$"        //固件程序版本
#define CREATE_TIME        "12/25/2019$"        //程序创建/修改时间W
#elif (1 == ULTRA_FLOW)
// 超声流量计
#define APP_VERSION        "RTU-2.24$"            //使用协议
#define RLS_VERSION        "CLL-181001$"    //固件程序版本
// #define RLS_VERSION        "ZQL-1903B1$"    //固件程序版本
// #define RLS_VERSION        "ZHL-1903B1$"    //固件程序版本
#define CREATE_TIME        "10/10/2018$"        //程序创建/修改时间W

#elif (2 == ULTRA_FLOW)
// 液介超声蒸发皿
#define APP_VERSION        "RTU-2.24$"            //使用协议
// #define RLS_VERSION        "Evap160701$"    //固件程序版本
// #define CREATE_TIME        "07/04/2016$"        //程序创建/修改时间W
#define RLS_VERSION        "EVA-181001$"    //固件程序版本
#define CREATE_TIME        "10/12/2017$"        //程序创建/修改时间W

#else
#error Use undefined.
#endif


//测量板变量
typedef struct BoardVariable{
    INT16U FixedMeasCnt;    //定测时间计数器，自减
    INT16U FixedReportCnt;//定测上报时间计数器，自增  20130223定测间隔，分；
    INT16U GapInDay;            //测量间隔的余数
    INT16U MinMeasPrd;            //预测最小间隔
    INT16U MaxMeasPrd;            //预测最大间隔
    INT16U ReportT;            //上报最大间隔
    
    INT8U GapDays;                //测量间隔中的天数
    INT8U PastDays;                //巡测时已经过去的天数
    INT8U MeasPreTime;        //测量提前时间，分钟
    INT8U MeasMode;                //
    
    INT8U DevNum;                    //
    INT8U MeterNum;                //
    INT8U SenNum;                    //
    INT8U MinUnit;                //
    INT8U GapNum;                    //
    INT8U numBrd;                  // 板号
    INT8U save;                    // 上报数据
    INT8U workMode;                  // 工作模式
    INT8U numStation;                //
    INT8U dummy;                    //
    INT8U ctrlSw;                //
    INT8U ctrlDly;                    //
} BOARD_VAR;

//测量板参数 定义属性
typedef struct BoardParameter{
    INT16U ID;                //测量板ID
    INT16U Belong;        //对象上层
    INT8U Class;            //对象大类
    INT8U SubClass;        //对象小类
    
    INT8U    Role;                //序号，表明角色
    INT8U MeasMode;   //测量板的测量模式
    INT8U SaveItem;        //上报数据选择
    INT8U FixedT;     //定测间隔的数值
    INT8U MinUnit;    //定测间隔的单位
    INT8U PrePowerOn;    //提前上电时间(分钟，只在节电模式下有效)
    INT8U P0Protocol; //串口0的接收协议
    INT8U P1Protocol; //串口1的接收协议
    INT8U GapNum;            //用于控制向显示器、管理板发送数据的频率；
                      //0－不发送到显示器、n－每n次向管理板发送一次；
    INT8U ReportSwitch;      //工作方式；1－自报，2－应答，用于数字传感器；

    INT8U NumStation; //所在站号;    2014/02/05 滤波开关 0－普通，1－开启；
    INT8U NumBoard;   //板号
    INT8U PwrControl; //传感器电源控制，0－常加电；
    INT8U WorkMode;   //工作方式，上报数据为:0－0x88，1－0x81；

    INT8U CtrlUnitSw; // 是否操作控制器 1=需要操作控制器
    INT8U CtrlUnitDly;   // 控制器等待延时，分钟
    INT16U BaudRate2;            //与测量模块的串口波特率

    INT16U LoopMeasB;    //巡测基准(小时：分钟)
    INT16U MeasT;            //巡测间隔(小时：分钟)
    INT16U MaxMeasPrd;//最大测量周期(小时：分钟)
    INT16U MinMeasPrd;//最小测量周期(小时：分钟)
    INT16U ReportT;        //测量板上对象上报数据的最大间隔时间(小时：分钟)
    INT8U crt_time[12];    //创建时间
    INT8U rls_ver[12];        //公司发布版本
    INT8U app_ver[12];        //程序版本
} BOARD_PARAM;

extern const BOARD_PARAM BoardPara;
extern BOARD_VAR BoardVar;

#endif

