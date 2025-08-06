#ifndef __DATABASE_H__
#define __DATABASE_H__


#include <drv_spi.h>
#include <spi_flash_sfud.h>
#include <fal.h>

#include <stdio.h>
#include <flashdb.h>
#include <stm32f1xx_hal.h>
#include "rtc.h"
#include "modbus.h"
#include "task.h"
///* KVDB object */
//struct fdb_kvdb kvdb1,kvdb2,kvdb3,kvdb4;

///* TSDB object */
//struct fdb_tsdb tsdb1,tsdb2,tsdb3;

/* KVDB object */



#define KVDB_NUM  10

typedef struct rwdatabase{
    uint16_t addr;
    uint8_t length;
    uint8_t *data;
}rwdatabase;


//����������ṹ������

/**
 * @brief      define the table struct
 */
typedef struct  
{
    uint16_t startAddress;              /**<starting address */
    uint16_t endAddress;                /**< �������ݽ�����ַ */
    uint8_t dataPartition;              /**< �������ݿ�ķ��� */
    uint8_t tableId;                    /**< ���ʶ */
    uint8_t maxCount;                   /**< ��������������� */
    uint8_t byteCount;                  /**< ����ÿ����������ֽ��� */
}index_table;
typedef index_table *index_table_t;             /**< ����������ṹ��ָ������ */

// ��������ṹ����
typedef struct {
    uint16_t dataAddress;
    uint8_t dataType;
    uint8_t dataLength;
} physic_table;

typedef struct {
    uint16_t dataAddress;
    float value;
} runingpara;

union float_endian_convert{
    float value;
    rt_uint8_t bytes[4];
};


extern void *databasesort[KVDB_NUM];

extern index_table_t globalTables;

extern size_t globalNumTables;

/* counts for simulated timestamp */


void lock(fdb_db_t db);
void unlock(fdb_db_t db);
fdb_time_t get_time(void);
int  database_init(void);



// ��̬�����ͳ�ʼ������������
index_table_t createIndexTableArray(size_t size) ;
// ��������������
void setIndexTableData(index_table_t tableArray, uint16_t start, uint16_t end, uint16_t partition,uint8_t id, uint8_t max, uint8_t byteCount);
// �洢���ݵ����ݿ�
//void storeToDatabase(DecodedModbusBody *data);
void storeToDatabase(rwdatabase *para);
// �����ݿ��м�������
//int retrieveFromDatabase(DecodedModbusBody *data);
int retrieveFromDatabase(rwdatabase *para);


#endif
