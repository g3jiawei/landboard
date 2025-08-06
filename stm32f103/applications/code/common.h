/******************************************************************************

Copyright (C), 2010-2020,西安兰特水电测控技术有限责任公司
All rights reserved.

File name:		common.h
Author:     	Warrior
Version:    	1.0
Date:					11/02/2010

Description:	简单数据类型的别名，各种常用常量的定义，常用宏定义。

Other:      	其他内容

Function List:主要函数列表，每条记录应包含函数名及功能简要说明
            	1. .........
            	2. .........

History:    	每条修改历史记录应包括修改者、日期、版本及修改内容简述
            	1.Mender:	Warrior
              	Date:		08/02/2011
              	Modify:	增、删与应用相关部分
            
******************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

/************************* 定义与编译器无关的数据类型 *************************/
typedef unsigned char		BOOL;				//布尔变量(字节)
typedef unsigned int		BOOLEAN;		//布尔变量(字)
typedef unsigned char		INT8U;			//无符号8位整型变量
typedef signed char			INT8S;			//有符号8位整型变量
typedef unsigned short	INT16U;			//无符号16位整型变量
typedef signed short		INT16S;			//有符号16位整型变量
typedef unsigned long		INT32U;			//无符号32位整型变量
typedef signed long			INT32S;			//有符号32位整型变量
typedef unsigned int		INTU;				//系统相关无符号整型变量
typedef signed int			INTS;				//系统相关有符号整型变量
typedef float						FP32;				//单精度浮点数（32位长度）
typedef double					FP64;				//双精度浮点数（64位长度）

typedef INT8S						BYTE;				//Define data types for backward ...
typedef INT8U						UBYTE;			//... compatibility to uC/OS V1.xx...
typedef INT16S					WORD;				//... Not actually needed for uC/OS-II.
typedef INT16U					UWORD;
typedef INT32S					LONG;
typedef INT32U					ULONG;


/*************************** 定义与编译器无关的常量 ***************************/
//#ifndef NUL
//#define NUL 			0
//#endif

//#ifndef TRUE
//#define TRUE			1
//#endif

//#ifndef FALSE
//#define FALSE			0
//#endif

//#ifndef ON
//#define ON				1
//#endif

//#ifndef OFF
//#define OFF				0
//#endif

//#ifndef SUCCESS
//#define SUCCESS		1
//#endif

//#ifndef FAILURE
//#define FAILURE		0
//#endif

//#ifndef ENABLE
//#define ENABLE		1
//#endif

//#ifndef DISABLE
//#define DISABLE		0
//#endif

//#ifndef NULL
//#define NULL		  ((void *)0)
//#endif

//#ifndef MAX_MIN
//#define MAX_INTS  0x7fff
//#define MAX_INTU	((INTU)(-1))

//#define MAX_INT32S		0x7fffffff
//#define MAX_INT32U		0xffffffff
//#define MAX_INT16S		0x7fff
//#define MAX_INT16U		0xffff
//#define MAX_INT8S			0x7f
//#define MAX_INT8U			0xff
//#endif


//#define MAX_PT		((void *)((INTU)(-1)))

//#define PI	(3.14159)

//#define MAX_FLOAT   (3.4028E38)
//#define MIN_FLOAT   (-3.4028E38)


//#define NULL_NUM  (-100)    //空
//#define NUM_REPLACE 20
//#define CELE_AIR		(299790000.0f)
//#define SPEED_TIME  (6.46e-5) //<0.08m，最小时间(0.10m)
//#define TEMP_MAX  (30)
//#define DEPTH_AREA_TIME  (1.938e-4) //<0.08m，最小时间(0.15m)
//#define VS		(1450.0)
//// #define DEPTH_AREA_NUM  (22)//闸前
//#define DEPTH_AREA_NUM  (24)//闸后
//#define POW_A  (1.66667)
//#define POW_X  (0.66667)



///**************************** 一些通用运算的宏定义 ****************************/
//#define	SETB(PORT, BIT_MASK)		((PORT) |=  (BIT_MASK))
//#define	CLRB(PORT, BIT_MASK)		((PORT) &= ~(BIT_MASK))
//#define	INVB(PORT, BIT_MASK)		((PORT) ^=  (BIT_MASK))

//#define	GETB(PORT, BIT_MASK)		((PORT)  &  (BIT_MASK))

//#define OFFSET(Struct, Member)   ((INT16U)(&(((Struct *)0)->Member)))	//结构中成员的偏移量
//#define SIZEOF(Struct, Member) (sizeof(((Struct *)0)->Member))				//结构中成员的长度

///**************************** 与应用相关的结构定义 ****************************/
////Object lookup
//typedef struct LookupRecord{
//	INT16U id;		//id
//	INT16U type;	//object
//	INT16U sn;		//sn
//	INT16U bytes;	//bytes per record
//	INT16U items;	//record per page
//	INT16U start;	//base page
//	INT8U *pb;		//data buffer/object parameter
//	INT8U *pvar;	//variable
//	INT16U sum;		//variable number
//	FP32 **pavg;   //
//}LOOKUP_RCD;


////数据存储结构
//typedef struct DataStore
//{
//	  INT8U Protocol;		//数据协议类型
//	  INT8U len;        //所存数据的长度
//	  INT16U src;       //信源
//	  INT16U sn;        //序列号
//	  INT8U type;       //数据类型
//	  INT16U id;        //对象编号
//	  INT8U pd[45];     //所存数据的数组
//}DATA_STORE;        	


////对象参数属性的结构体
//typedef struct ParameterProperty
//{
//	INT8U code;							//属性编码
//	INT8U size;							//属性长度
//	INT8U skewing;					//属性在对象结构体中的偏移量
//	INT8U feature;					//属性特征：读写，存储区；
//	INT8U (*set)(INT8U *pdest, INT8U const *psrc, INT8U len);
//	INT8U (*get)(INT8U *const pdest, INT8U const *psrc, INT8U len);
//}PARA_PROP;

//typedef struct ParamPropTable{
//	PARA_PROP const *pParamTable;
//	INT8U num;
//	INT8U dummy;
//}PARAM_PROP_TABLE;


////类的公共信息
//typedef struct ClassInformation
//{
//	INT8U Family;		        //大类
//	INT8U Genus;		        //对象类型编码，子类
//	
//	INT8U PropTableSn;		//属性表序号
//	INT8U DataSrcSn;	        //数据源编号
//	
//	INT8U FormulaInit;		//初始化公式编号
//	INT8U Formula;		        //转换公式编号
//	INT8U FormulaBase;		//基准设置公式编号
//	
//	INT8U InPhyNum;		        //输入物理量的数量
//	INT8U PhyIn1;		        //第一个输入物理量编码
//	INT8U PhyIn2;
//	INT8U PhyIn3;
//	
//	INT8U OutPhyNum;	        //输出物理量的数量
//	INT8U PhyOut1;		        //第一个输出物理量编码
//	INT8U PhyOut2;
//	INT8U PhyOut3;
//	INT8U PhyOut4;
//}CLASS_INFO;


//typedef struct FormulaParameter
//{
//	FP32 *par;		//参数
//	FP32 *pbase;	//基准
//	FP32 *pinit;	//初值
//	FP32 *pv;			//变量数据
//	FP32 *pre;		//结果
//	FP32 *pout;		//输出校验
//	//FP32 *pin;		//输入校验
//	LOOKUP_RCD *pr;	//设施结构
//	CLASS_INFO const*pinfo;	//类信息表
//	void* paux;     //特殊辅助参数
//}FORMULA_PARA;



//typedef struct MovingAverage
//{
//  INT8U sn;		//current
//  INT8U num;	//total
//  FP32* pa;
//	FP32 ft;		//add up
//}MOVING_AVG;


//typedef struct flow_rate_fliter
//{
//  FP32 vc;  //当前流速
//  FP32 v;   //前一次流速
//  FP32 q;   //前一次流量
//  FP32 k;   //k0，平均值
//  FP32 d;   //d0，均方差
//  FP32 r;   //管道半径
//  FP32 v0;  //流速初值
//  FP32 a;   //均方差的系数
//  FP32 b;   //最小带宽
//  INT8U n;  //最大次数
//  INT8U i;  //计数器
//  INT8U s;  //状态稳定标志
//  INT8U sc; //本次状态
//}FLOW_FLITER;

//typedef struct temperature_fliter
//{
//  FP32 vc;  //当前流速
//  FP32 v;   //前一次流速
//  FP32 v0;  //稳定流速
//  FP32 tc;  //当前温度
//  FP32 t;   //前一次温度
//  FP32 t0;  //稳定温度
//  FP32 d;   //d0，均方差
//  FP32 a;   //均方差的系数
//  FP32 b;   //最小带宽
//  INT8U n;  //最大次数
//  INT8U i;  //计数器
//  INT8U s;  //状态稳定标志
//  INT8U sc; //本次状态
//}T_FLITER;


//typedef struct temperature_flow_fliter
//{
//  FP32 vc;  //当前流速
//  FP32 v0;  //稳定流速
//  FP32 tc2; //前两次温度  i-2;
//  FP32 tc1; //前一次温度  i-1;
//  FP32 tc;  //当前温度
//  FP32 t0;  //稳定温度
//  FP32 d;   //d0，均方差
//  FP32 b;   //最小带宽
//  FP32 b0;   //差值的差值的最小带宽
//  INT8U n;  //最大次数
//  INT8U i;  //计数器
//  INT8U s;  //状态稳定标志
//  INT8U sc; //本次状态
//}T_FR_FLITER;


//typedef struct USART_Static_Variable
//{
//  INT8U *pd;
//  INT8U len;
//  INT8U sn;
//  INT8U s;    //接收允许
//  INT8U port;
//  INT8U num;  //发送长度
//  INT8U type; //协议类型
//  INT8U elec; //enable
//  INT8U dummy; //
//}USART_PORT;


//typedef struct difference_based_data_processing{
//	FP32 *pv;		// 实测值序列
//	FP32 dmax;
//	FP32 dmin;
//	FP32 cv;
//	FP32 tc0;
//	FP32 tc1;
//	FP32 d0;	// data0
//	FP32 d1;	// data1
//	FP32 dc0;	// cv - mv1
//	FP32 dc1;	// mv1 - mv2
//	FP32 mv2;	// 实测值 Tc(n-2, 1)
//	FP32 mv1;	// 实测值 Tc(n-1, 1)

//	INT8U n;	// 序列长度		20160709 方差更新比重
//	INT8U cn;	// current							最大门限替代次数
//	INT8U mc;	// 替代次数
//	INT8U mmc;	// 替代次数最大值			方差
//	INT8U s;	// sign
//	INT8U alt;	// 数据超限替代次数
//}DIFF_PROCESS;

//typedef struct data_preprocessing{
//	FP32 *pv;		// 数据序列

//	FP32 k;
//	FP32 b;
//	FP32 r;		// 趋势、常数及相关系数平方
//	FP32 a0;
//	FP32 a1;
//	FP32 a2;
//	FP32 deno;	// nb0 - b1^2
//	FP32 b0;	// b0
//	FP32 b1;
//	FP32 dt;	// 测量间隔
//	FP32 Smax;
//	FP32 Smin;	// 上下限
//	FP32 Dmin;	// 一次差分最小方差
//	FP32 Dmax;	// 二次差分最大方差
//	FP32 cv;		// 当前实测值
//	FP32 pc;		// 当前输出值
//	FP32 pc1;		// 上一个输出值
//	FP32 sc2;		// 上上一个经过门限处理的实测值
//	FP32 sc1;		// 上一个经过门限处理的实测值
//	FP32 d0;		// 一次差分方差更新值

//	INT8U n;	// current
//	INT8U nd;	// 处理后数据序列长度
//	INT8U mc0;	// 门限替代次数
//	INT8U mc;	// 替代次数最大值
//	INT8U nc0;	// 方差处理替代次数
//	INT8U nc;	// 替代次数最大值
//	INT8U m;	// 一次方差更新比重个数
//	INT8U s;	// sign

//}DATA_PREPROCESSING;



//typedef struct ultra_open_channel_flow
//{
//  FP32* pb;   //bi-底宽
//  FP32* ph;   //hi-声路高度
//  FP32* pdh;  //dhi-声路高差
//  FP32* pfh;  //声路淹没深度+冗余高度
//  FP32* plnh;  //声路高度的对数
//  FP32 t0;    //上一次垂直声路时间
//  INT8U s;    //1-初始化完成
//}U_OPEN_CHNNL;





#endif

/*********************************EOF*****************************************/

