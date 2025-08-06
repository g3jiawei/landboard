#include <common.h>
#include "dht11.h"
#include "flashdb.h"
#include "database.h"

/*定义数据存储数组*/

MSR_FRE             msr_fre_value[MAX_SEN];                     //渗压计存储数组
MSR_RESISTANCE      msr_resistance_value[MAX_SEN];              //差阻存储数组
float               msr_current_value[MAX_SEN];                 //标准电流存储数组
MSR_OTHER           msr_other_value[MAX_SEN/2];                 //其它测量值存储数组
MSR_ENVIRONMENT     msr_environment_value;                      //环境量存储结构体
int                 msr_rain_value;                             //雨量变量

PHYSICS para_physics[100];


