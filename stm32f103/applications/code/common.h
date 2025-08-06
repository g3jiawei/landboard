/******************************************************************************

Copyright (C), 2010-2020,��������ˮ���ؼ����������ι�˾
All rights reserved.

File name:		common.h
Author:     	Warrior
Version:    	1.0
Date:					11/02/2010

Description:	���������͵ı��������ֳ��ó����Ķ��壬���ú궨�塣

Other:      	��������

Function List:��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
            	1. .........
            	2. .........

History:    	ÿ���޸���ʷ��¼Ӧ�����޸��ߡ����ڡ��汾���޸����ݼ���
            	1.Mender:	Warrior
              	Date:		08/02/2011
              	Modify:	����ɾ��Ӧ����ز���
            
******************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

/************************* ������������޹ص��������� *************************/
typedef unsigned char		BOOL;				//��������(�ֽ�)
typedef unsigned int		BOOLEAN;		//��������(��)
typedef unsigned char		INT8U;			//�޷���8λ���ͱ���
typedef signed char			INT8S;			//�з���8λ���ͱ���
typedef unsigned short	INT16U;			//�޷���16λ���ͱ���
typedef signed short		INT16S;			//�з���16λ���ͱ���
typedef unsigned long		INT32U;			//�޷���32λ���ͱ���
typedef signed long			INT32S;			//�з���32λ���ͱ���
typedef unsigned int		INTU;				//ϵͳ����޷������ͱ���
typedef signed int			INTS;				//ϵͳ����з������ͱ���
typedef float						FP32;				//�����ȸ�������32λ���ȣ�
typedef double					FP64;				//˫���ȸ�������64λ���ȣ�

typedef INT8S						BYTE;				//Define data types for backward ...
typedef INT8U						UBYTE;			//... compatibility to uC/OS V1.xx...
typedef INT16S					WORD;				//... Not actually needed for uC/OS-II.
typedef INT16U					UWORD;
typedef INT32S					LONG;
typedef INT32U					ULONG;


/*************************** ������������޹صĳ��� ***************************/
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


//#define NULL_NUM  (-100)    //��
//#define NUM_REPLACE 20
//#define CELE_AIR		(299790000.0f)
//#define SPEED_TIME  (6.46e-5) //<0.08m����Сʱ��(0.10m)
//#define TEMP_MAX  (30)
//#define DEPTH_AREA_TIME  (1.938e-4) //<0.08m����Сʱ��(0.15m)
//#define VS		(1450.0)
//// #define DEPTH_AREA_NUM  (22)//բǰ
//#define DEPTH_AREA_NUM  (24)//բ��
//#define POW_A  (1.66667)
//#define POW_X  (0.66667)



///**************************** һЩͨ������ĺ궨�� ****************************/
//#define	SETB(PORT, BIT_MASK)		((PORT) |=  (BIT_MASK))
//#define	CLRB(PORT, BIT_MASK)		((PORT) &= ~(BIT_MASK))
//#define	INVB(PORT, BIT_MASK)		((PORT) ^=  (BIT_MASK))

//#define	GETB(PORT, BIT_MASK)		((PORT)  &  (BIT_MASK))

//#define OFFSET(Struct, Member)   ((INT16U)(&(((Struct *)0)->Member)))	//�ṹ�г�Ա��ƫ����
//#define SIZEOF(Struct, Member) (sizeof(((Struct *)0)->Member))				//�ṹ�г�Ա�ĳ���

///**************************** ��Ӧ����صĽṹ���� ****************************/
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


////���ݴ洢�ṹ
//typedef struct DataStore
//{
//	  INT8U Protocol;		//����Э������
//	  INT8U len;        //�������ݵĳ���
//	  INT16U src;       //��Դ
//	  INT16U sn;        //���к�
//	  INT8U type;       //��������
//	  INT16U id;        //������
//	  INT8U pd[45];     //�������ݵ�����
//}DATA_STORE;        	


////����������ԵĽṹ��
//typedef struct ParameterProperty
//{
//	INT8U code;							//���Ա���
//	INT8U size;							//���Գ���
//	INT8U skewing;					//�����ڶ���ṹ���е�ƫ����
//	INT8U feature;					//������������д���洢����
//	INT8U (*set)(INT8U *pdest, INT8U const *psrc, INT8U len);
//	INT8U (*get)(INT8U *const pdest, INT8U const *psrc, INT8U len);
//}PARA_PROP;

//typedef struct ParamPropTable{
//	PARA_PROP const *pParamTable;
//	INT8U num;
//	INT8U dummy;
//}PARAM_PROP_TABLE;


////��Ĺ�����Ϣ
//typedef struct ClassInformation
//{
//	INT8U Family;		        //����
//	INT8U Genus;		        //�������ͱ��룬����
//	
//	INT8U PropTableSn;		//���Ա����
//	INT8U DataSrcSn;	        //����Դ���
//	
//	INT8U FormulaInit;		//��ʼ����ʽ���
//	INT8U Formula;		        //ת����ʽ���
//	INT8U FormulaBase;		//��׼���ù�ʽ���
//	
//	INT8U InPhyNum;		        //����������������
//	INT8U PhyIn1;		        //��һ����������������
//	INT8U PhyIn2;
//	INT8U PhyIn3;
//	
//	INT8U OutPhyNum;	        //���������������
//	INT8U PhyOut1;		        //��һ���������������
//	INT8U PhyOut2;
//	INT8U PhyOut3;
//	INT8U PhyOut4;
//}CLASS_INFO;


//typedef struct FormulaParameter
//{
//	FP32 *par;		//����
//	FP32 *pbase;	//��׼
//	FP32 *pinit;	//��ֵ
//	FP32 *pv;			//��������
//	FP32 *pre;		//���
//	FP32 *pout;		//���У��
//	//FP32 *pin;		//����У��
//	LOOKUP_RCD *pr;	//��ʩ�ṹ
//	CLASS_INFO const*pinfo;	//����Ϣ��
//	void* paux;     //���⸨������
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
//  FP32 vc;  //��ǰ����
//  FP32 v;   //ǰһ������
//  FP32 q;   //ǰһ������
//  FP32 k;   //k0��ƽ��ֵ
//  FP32 d;   //d0��������
//  FP32 r;   //�ܵ��뾶
//  FP32 v0;  //���ٳ�ֵ
//  FP32 a;   //�������ϵ��
//  FP32 b;   //��С����
//  INT8U n;  //������
//  INT8U i;  //������
//  INT8U s;  //״̬�ȶ���־
//  INT8U sc; //����״̬
//}FLOW_FLITER;

//typedef struct temperature_fliter
//{
//  FP32 vc;  //��ǰ����
//  FP32 v;   //ǰһ������
//  FP32 v0;  //�ȶ�����
//  FP32 tc;  //��ǰ�¶�
//  FP32 t;   //ǰһ���¶�
//  FP32 t0;  //�ȶ��¶�
//  FP32 d;   //d0��������
//  FP32 a;   //�������ϵ��
//  FP32 b;   //��С����
//  INT8U n;  //������
//  INT8U i;  //������
//  INT8U s;  //״̬�ȶ���־
//  INT8U sc; //����״̬
//}T_FLITER;


//typedef struct temperature_flow_fliter
//{
//  FP32 vc;  //��ǰ����
//  FP32 v0;  //�ȶ�����
//  FP32 tc2; //ǰ�����¶�  i-2;
//  FP32 tc1; //ǰһ���¶�  i-1;
//  FP32 tc;  //��ǰ�¶�
//  FP32 t0;  //�ȶ��¶�
//  FP32 d;   //d0��������
//  FP32 b;   //��С����
//  FP32 b0;   //��ֵ�Ĳ�ֵ����С����
//  INT8U n;  //������
//  INT8U i;  //������
//  INT8U s;  //״̬�ȶ���־
//  INT8U sc; //����״̬
//}T_FR_FLITER;


//typedef struct USART_Static_Variable
//{
//  INT8U *pd;
//  INT8U len;
//  INT8U sn;
//  INT8U s;    //��������
//  INT8U port;
//  INT8U num;  //���ͳ���
//  INT8U type; //Э������
//  INT8U elec; //enable
//  INT8U dummy; //
//}USART_PORT;


//typedef struct difference_based_data_processing{
//	FP32 *pv;		// ʵ��ֵ����
//	FP32 dmax;
//	FP32 dmin;
//	FP32 cv;
//	FP32 tc0;
//	FP32 tc1;
//	FP32 d0;	// data0
//	FP32 d1;	// data1
//	FP32 dc0;	// cv - mv1
//	FP32 dc1;	// mv1 - mv2
//	FP32 mv2;	// ʵ��ֵ Tc(n-2, 1)
//	FP32 mv1;	// ʵ��ֵ Tc(n-1, 1)

//	INT8U n;	// ���г���		20160709 ������±���
//	INT8U cn;	// current							��������������
//	INT8U mc;	// �������
//	INT8U mmc;	// ����������ֵ			����
//	INT8U s;	// sign
//	INT8U alt;	// ���ݳ����������
//}DIFF_PROCESS;

//typedef struct data_preprocessing{
//	FP32 *pv;		// ��������

//	FP32 k;
//	FP32 b;
//	FP32 r;		// ���ơ����������ϵ��ƽ��
//	FP32 a0;
//	FP32 a1;
//	FP32 a2;
//	FP32 deno;	// nb0 - b1^2
//	FP32 b0;	// b0
//	FP32 b1;
//	FP32 dt;	// �������
//	FP32 Smax;
//	FP32 Smin;	// ������
//	FP32 Dmin;	// һ�β����С����
//	FP32 Dmax;	// ���β����󷽲�
//	FP32 cv;		// ��ǰʵ��ֵ
//	FP32 pc;		// ��ǰ���ֵ
//	FP32 pc1;		// ��һ�����ֵ
//	FP32 sc2;		// ����һ���������޴����ʵ��ֵ
//	FP32 sc1;		// ��һ���������޴����ʵ��ֵ
//	FP32 d0;		// һ�β�ַ������ֵ

//	INT8U n;	// current
//	INT8U nd;	// ������������г���
//	INT8U mc0;	// �����������
//	INT8U mc;	// ����������ֵ
//	INT8U nc0;	// ������������
//	INT8U nc;	// ����������ֵ
//	INT8U m;	// һ�η�����±��ظ���
//	INT8U s;	// sign

//}DATA_PREPROCESSING;



//typedef struct ultra_open_channel_flow
//{
//  FP32* pb;   //bi-�׿�
//  FP32* ph;   //hi-��·�߶�
//  FP32* pdh;  //dhi-��·�߲�
//  FP32* pfh;  //��·��û���+����߶�
//  FP32* plnh;  //��·�߶ȵĶ���
//  FP32 t0;    //��һ�δ�ֱ��·ʱ��
//  INT8U s;    //1-��ʼ�����
//}U_OPEN_CHNNL;





#endif

/*********************************EOF*****************************************/

