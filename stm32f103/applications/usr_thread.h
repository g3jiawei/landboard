#ifndef __USR_THREAD_H__
#define __USR_THREAD_H__


/* 通信口解码线程 */
void decodecom_thread_entry(void *para);

/* 测量口解码线程 */
void decodemea_thread_entry(void *para);

/* 通信口发送线程 */
void comsend_thread_entry(void *para);

/* 测量口发送线程 */
void measend_thread_entry(void *para );

/* 读取并存储温湿度线程 */
//void getdht11_thread_entry(void *para);

/* 测量模式线程 */
void meamode_thread_entry(void *para);

/* 测量线程 */
void measure_thread_entry(void *para);

#endif
