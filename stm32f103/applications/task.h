#ifndef     __TASK_H__
#define     __TASK_H__

#include "modbus.h"
#include "database.h"
#include "ipc/workqueue.h"
#include "uart_app.h"




/*外部使用的函数声明*/
void task_index(struct rt_work *work,void *workdata);
void task_command(struct rt_work *work,void *workdata);
void task_runpara(struct rt_work *work,void *workdata);
void task_status(struct rt_work *work,void *workdata);
void task_aim(struct rt_work *work,void *workdata);

void task_sort(DecodedModbusBody *body,task_msg_t msg);

#endif
