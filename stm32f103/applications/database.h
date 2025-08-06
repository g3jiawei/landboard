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


//定义索引表结构体类型

/**
 * @brief      define the table struct
 */
typedef struct  
{
    uint16_t startAddress;              /**<starting address */
    uint16_t endAddress;                /**< 表内数据结束地址 */
    uint8_t dataPartition;              /**< 表在数据库的分区 */
    uint8_t tableId;                    /**< 表标识 */
    uint8_t maxCount;                   /**< 表内数据最大条数 */
    uint8_t byteCount;                  /**< 表内每条数据最大字节数 */
}index_table;
typedef index_table *index_table_t;             /**< 定义索引表结构体指针类型 */

// 物理量表结构定义
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



// 动态创建和初始化索引表数组
index_table_t createIndexTableArray(size_t size) ;
// 设置索引表数据
void setIndexTableData(index_table_t tableArray, uint16_t start, uint16_t end, uint16_t partition,uint8_t id, uint8_t max, uint8_t byteCount);
// 存储数据到数据库
//void storeToDatabase(DecodedModbusBody *data);
void storeToDatabase(rwdatabase *para);
// 从数据库中检索数据
//int retrieveFromDatabase(DecodedModbusBody *data);
int retrieveFromDatabase(rwdatabase *para);


#endif
