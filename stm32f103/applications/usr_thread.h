#ifndef __USR_THREAD_H__
#define __USR_THREAD_H__


/* ͨ�ſڽ����߳� */
void decodecom_thread_entry(void *para);

/* �����ڽ����߳� */
void decodemea_thread_entry(void *para);

/* ͨ�ſڷ����߳� */
void comsend_thread_entry(void *para);

/* �����ڷ����߳� */
void measend_thread_entry(void *para );

/* ��ȡ���洢��ʪ���߳� */
//void getdht11_thread_entry(void *para);

/* ����ģʽ�߳� */
void meamode_thread_entry(void *para);

/* �����߳� */
void measure_thread_entry(void *para);

#endif
