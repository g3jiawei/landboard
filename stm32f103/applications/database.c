#include <database.h>
#include "task.h"
#define FDB_LOG_TAG "[main]"


#define FDB_LOG_TAG "[main]"



struct fdb_kvdb kvdb1,kvdb2,kvdb3,kvdb4;
struct fdb_tsdb tsdb1,tsdb2,tsdb3;



void *databasesort[KVDB_NUM]= {NULL,&kvdb1,&kvdb2,&kvdb3,&kvdb4,&tsdb1,&tsdb2,&tsdb3};

/* counts for simulated timestamp */
//static int counts = 0;

void lock(fdb_db_t db)
{
    __disable_irq();
}

void unlock(fdb_db_t db)
{
    __enable_irq();
}

fdb_time_t get_time(void)
{
    /* Using the counts instead of timestamp.
     * Please change this function to return RTC time.
     */
//    fdb_time_t temp2 = 0;
//    rtc_get_time();
//    temp2 = Bin2BCD(calendar.year);
//    temp2 <<= 8;
//    temp2 |= Bin2BCD(calendar.month);
//    temp2 <<= 8;
//    temp2 |= Bin2BCD(calendar.date);
//    temp2 <<= 8;
//    temp2 |= Bin2BCD(calendar.hour);
//    temp2 <<= 8;
//    temp2 = Bin2BCD(calendar.min);
//    temp2 <<= 8;    
//    temp2 |= Bin2BCD(calendar.sec);
//    return temp2;
    uint32_t seccount = 0;
    seccount = RTC->CNTH;       /* �õ��������е�ֵ(������) */
    seccount <<= 16;
    seccount += RTC->CNTL;
    return seccount;


}

//static bool query_cb(fdb_tsl_t tsl, void *arg)
//{
//    struct fdb_blob blob;
//    struct env_status status;
//    fdb_tsdb_t db = arg;

//    fdb_blob_read((fdb_db_t) db, fdb_tsl_to_blob(tsl, fdb_blob_make(&blob, &status, sizeof(status))));
//    FDB_INFO("[query_cb] queried a TSL: time: %lld, temp: %d, humi: %d\n", tsl->time, status.temp, status.humi);

//    return false;
//}

int  database_init()
{        
        fdb_err_t result;
//        struct fdb_default_kv default_kv;

//        default_kv.kvs = default_kv_table;
//        default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
    
    
        rt_hw_spi_device_attach("spi2","spi20",GPIOB,GPIO_PIN_12);                //spi20�豸���ص�����spi2��
    
        if(RT_NULL == rt_sfud_flash_probe("NM25Q128","spi20"))                        //���NM25Q128���ص�spi20��
            {
                return -RT_ERROR;
            }
        
        //FLASH������ʼ��
        fal_init();

        /* Key-Value database initialization
         *
         *       &kvdb: database object
         *       "env": database name
         * "kvdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         * &default_kv: The default KV nodes. It will auto add to KVDB when first initialize successfully.
         *        NULL: The user data if you need, now is empty.
         */
            result = fdb_kvdb_init((fdb_kvdb_t)databasesort[1], "phy_tun", "kvdb1", NULL, NULL);
            result = fdb_kvdb_init((fdb_kvdb_t)databasesort[2], "pa_prj", "kvdb2", NULL, NULL);
            result = fdb_kvdb_init((fdb_kvdb_t)databasesort[3], "para", "kvdb3", NULL, NULL);
            result = fdb_kvdb_init((fdb_kvdb_t)databasesort[4], "kv4", "kvdb4", NULL, NULL);
        if (result != FDB_NO_ERR) 
            {
                return -2;
            }
//fdb_kvdb_deinit((fdb_kvdb_t)databasesort[1]);
//fdb_kvdb_deinit((fdb_kvdb_t)databasesort[2]);
//fdb_kvdb_deinit((fdb_kvdb_t)databasesort[3]);
//fdb_kvdb_deinit((fdb_kvdb_t)databasesort[4]);
        /* Time series database initialization
         *
         *       &tsdb: database object
         *       "log": database name
         * "fdb_tsdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         *    get_time: The get current timestamp function.
         *         128: maximum length of each log
         *        NULL: The user data if you need, now is empty.
         */
        result = fdb_tsdb_init((fdb_tsdb_t)databasesort[5], "data", "tsdb1", get_time, 128, NULL);
        result = fdb_tsdb_init((fdb_tsdb_t)databasesort[6], "env", "tsdb2", get_time, 12, NULL);
        result = fdb_tsdb_init((fdb_tsdb_t)databasesort[7], "rain", "tsdb3", get_time, 8, NULL);
        /* read last saved time for simulated timestamp */
       // fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_GET_LAST_TIME, &counts);

        if (result != FDB_NO_ERR) {
            return -3;
        }

        return RT_EOK;
//fdb_tsdb_deinit((fdb_tsdb_t)databasesort[5]);
}

// ȫ��������ͱ�����
index_table_t globalTables;

size_t globalNumTables;



// ��̬�����ͳ�ʼ������������
index_table_t createIndexTableArray(size_t size) {
    return rt_calloc(size, sizeof(index_table));
}

// ��������������
void setIndexTableData(index_table_t tableArray,  uint16_t start, uint16_t end, uint16_t partition, uint8_t id,uint8_t max, uint8_t byteCount) {
    tableArray->tableId = id;
    tableArray->startAddress = start;
    tableArray->endAddress = end;
    tableArray->dataPartition = partition; // ���ݿ����
    tableArray->maxCount = max;
    tableArray->byteCount = byteCount;
}

// �ҵ�����startingAddress�ı�����������������������򷵻�-1
int16_t findTableIndex(index_table_t tables, size_t numTables, uint16_t startingAddress) {
    for (size_t i = 0; i < numTables; i++) {
        if (startingAddress >= tables[i].startAddress && startingAddress <= tables[i].endAddress) {
            return i;
        }
    }
    return -1;
}

// ����ַת��Ϊ�ַ�����ʾ����ʽΪ "4" + 4λʮ�����Ƶ�ַ
void addressToString(char *str, uint16_t address) {
    rt_snprintf(str, 8, "4%04X", address);
}

// �����ϣ����
uint16_t hashFunction(index_table_t table, uint16_t startingAddress) {
    uint16_t index = startingAddress - table->startAddress;
    return table->startAddress + (index * table->byteCount);
}

// ����dataPartitionȷ�����ݿ����
void* getDatabaseFromPartition(uint16_t partition) {
    if (partition < KVDB_NUM) {
        return databasesort[partition];
    }
    return NULL;
}

// У��dataPartition
int validateDataPartition(uint16_t dataPartition) {
    if (dataPartition >= 1 && dataPartition <= 4) {
        return 1; // Valid
    } else {
        return 0; // Invalid
    }
}

// �洢���ݵ����ݿ�
void storeToDatabase(rwdatabase *para) {
    struct fdb_blob blob;
    int16_t tableIndex = findTableIndex(globalTables, globalNumTables, para->addr);
    if (tableIndex != -1) {
        index_table_t table = &globalTables[tableIndex];
        if (!validateDataPartition(table->dataPartition)) {
            rt_kprintf("Invalid dataPartition: %d\n", table->dataPartition);
            return;
        }
        void *db = getDatabaseFromPartition(table->dataPartition);
        if (db == NULL) {
            rt_kprintf("Invalid dataPartition: 0x%X\n", table->dataPartition);
            return;
        }
//        uint16_t physicalAddress = hashFunction(table, data->startingAddress);
        char addressStr[8];
        addressToString(addressStr, para->addr);

        fdb_kv_set_blob((struct fdb_kvdb*)db, addressStr, fdb_blob_make(&blob, para->data, para->length));
        rt_kprintf("Data stored key : %s\n", addressStr);
    } else {
        rt_kprintf("No table contains the address 0x%X\n", para->addr);
    }
}

// �����ݿ��м�������
int retrieveFromDatabase(rwdatabase *para) {
    struct fdb_blob retrievedBlob;
    
    int16_t tableIndex = findTableIndex(globalTables, globalNumTables, para->addr);
    if (tableIndex != -1) {
        index_table_t table = &globalTables[tableIndex];
        if (!validateDataPartition(table->dataPartition)) {
            rt_kprintf("Invalid dataPartition: %d\n", table->dataPartition);
            return -1;
        }
        
        void *db = getDatabaseFromPartition(table->dataPartition);          //��ȡ���ݿ����
        if (db == NULL) {
            rt_kprintf("Invalid dataPartition: 0x%X\n", table->dataPartition);
            return -1;
        }
        para->data = (uint8_t*)rt_malloc(table->byteCount * sizeof(uint8_t));    //��data�����ڴ�
        if (para->data == NULL) {
            rt_kprintf("Failed to allocate memory for data array.\n");
            return -1;
        }
        
        char addressStr[8];
        addressToString(addressStr, para->addr);
        para->length = fdb_kv_get_blob((struct fdb_kvdb*)db, addressStr, fdb_blob_make(&retrievedBlob, para->data, table->byteCount));
        
        if (para->length > 0) {
            return 0; // Success
        } else {
            rt_kprintf("Failed to retrieve data for address %s\n", addressStr);
            return -1; // Failure
        }
    } else {
        rt_kprintf("No table contains the address 0x%X\n", para->addr);
        return -1; // Failure
    }
}
//// �洢���ݵ����ݿ�
//void storeToDatabase(DecodedModbusBody *data) {
//    struct fdb_blob blob;
//    int tableIndex = findTableIndex(globalTables, globalNumTables, data->startingAddress);
//    if (tableIndex != -1) {
//        index_table_t *table = &globalTables[tableIndex];
//        if (!validateDataPartition(table->dataPartition)) {
//            rt_kprintf("Invalid dataPartition: %d\n", table->dataPartition);
//            return;
//        }
//        void *db = getDatabaseFromPartition(table->dataPartition);
//        if (db == NULL) {
//            rt_kprintf("Invalid dataPartition: 0x%X\n", table->dataPartition);
//            return;
//        }
//        char addressStr[8];
//        addressToString(addressStr, data->startingAddress);

//        fdb_kv_set_blob((struct fdb_kvdb*)db, addressStr, fdb_blob_make(&blob, data->data, data->count));
//        rt_kprintf("Data stored key : %s\n", addressStr);
//    } else {
//        rt_kprintf("No table contains the address 0x%X\n", data->startingAddress);
//    }
//}

//// �����ݿ��м�������
//int retrieveFromDatabase(DecodedModbusBody *data) {
//    struct fdb_blob retrievedBlob;
//    
//    int tableIndex = findTableIndex(globalTables, globalNumTables, data->startingAddress);
//    if (tableIndex != -1) {
//        index_table_t *table = &globalTables[tableIndex];
//        if (!validateDataPartition(table->dataPartition)) {
//            printf("Invalid dataPartition: %d\n", table->dataPartition);
//            return -1;
//        }
//        
//        void *db = getDatabaseFromPartition(table->dataPartition);          //��ȡ���ݿ����
//        if (db == NULL) {
//            printf("Invalid dataPartition: 0x%X\n", table->dataPartition);
//            return -1;
//        }
//        data->count = table->byteCount;                                      //��ȡ�洢���ݵ��ֽ���
//        data->byteCount = table->byteCount;
//        data->data = (uint8_t*)rt_malloc(data->count * sizeof(uint8_t));    //��data�����ڴ�
//        if (data->data == NULL) {
//            printf("Failed to allocate memory for data array.\n");
//            return -1;
//        }
//        
//        char addressStr[8];
//        addressToString(addressStr, data->startingAddress);
//        if (fdb_kv_get_blob((struct fdb_kvdb*)db, addressStr, fdb_blob_make(&retrievedBlob, data->data, data->count)) > 0) {
//            return 0; // Success
//        } else {
//            printf("Failed to retrieve data for address %s\n", addressStr);
//            return -1; // Failure
//        }
//    } else {
//        printf("No table contains the address 0x%X\n", data->startingAddress);
//        return -1; // Failure
//    }
//}
