#include <common.h>
#include "dht11.h"
#include "flashdb.h"
#include "database.h"

/*�������ݴ洢����*/

MSR_FRE             msr_fre_value[MAX_SEN];                     //��ѹ�ƴ洢����
MSR_RESISTANCE      msr_resistance_value[MAX_SEN];              //����洢����
float               msr_current_value[MAX_SEN];                 //��׼�����洢����
MSR_OTHER           msr_other_value[MAX_SEN/2];                 //��������ֵ�洢����
MSR_ENVIRONMENT     msr_environment_value;                      //�������洢�ṹ��
int                 msr_rain_value;                             //��������

PHYSICS para_physics[100];


