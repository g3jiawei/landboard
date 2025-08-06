// modbus.h
#ifndef __MODBUS_H__
#define __MODBUS_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <board.h>


#define MAX_CODE_COUNT 3

typedef struct {
    int type01Codes[MAX_CODE_COUNT];
    int type03Codes[MAX_CODE_COUNT];
    int type05Codes[MAX_CODE_COUNT];
    int type06Codes[MAX_CODE_COUNT];
    int type0FCodes[MAX_CODE_COUNT];
    int type10Codes[MAX_CODE_COUNT];
} FunctionCodes;


/**< 定义各个功能区的地址范围 */

#define MB_START_ADDR       0                   /**< 定义通用起始地址 */

/***************************输入寄存器*******************************************/

#define MB_DATA_END_ADDR    400                 /**< 数据结束地址，起始地址为 MB_START_ADDR */

/***************************保持寄存器*******************************************/

#define MB_AIM_END_ADDR         100             /**< 目标值结束地址，起始地址为 MB_START_ADDR */
#define MB_RUNPARA_START_ADDR   101             /**< 运行参数起始地址 */
#define MB_RUNPARA_END_ADDR     300             /**< 运行参数结束地址 */
#define MB_PARA_START_ADDR      301             /**< 参数起始地址 */
#define MB_PARA_END_ADDR        2000            /**< 参数结束地址 */

/***************************线圈*******************************************/

#define MB_COIL_END_ADDR    95                  /**< 线圈结束地址，起始地址为MB_START_ADDR */

/***************************输入离散量*******************************************/

#define MB_INPUT_END_ADDR   95                  /**< 输入开关量结束地址，起始地址为MB_START_ADDR */



// 全局变量
extern FunctionCodes g_codes;


typedef struct {
    uint8_t address; // 设备地址
} ModbusHeader;

typedef struct {
    uint8_t function;    // 功能码
    const uint8_t *data; // 指向剩余包体数据的指针
    size_t length;       // 剩余包体数据的长度
} ModbusBody;

typedef struct {
    uint8_t crc[2]; // CRC校验码
} ModbusFooter;

typedef struct {
    ModbusHeader header;
    ModbusBody body;
    ModbusFooter footer;
} ModbusPacket;

typedef struct { 
    ModbusBody body; // 包体
    uint8_t address; // 信宿地址
} DecodedModbusPacket;

typedef struct {
    uint8_t deviceAddress;      // 信宿
    uint8_t function;           // 功能码
    uint16_t startingAddress;   // 起始地址
    uint16_t quantity;          // 寄存器/线圈数量
    uint8_t byteCount;          // 字节数
    uint8_t *data;              // 指向数据的数组
    uint8_t count;              // 数组中的元素数量
} DecodedModbusBody;

typedef struct {
    char keyChar[6]; // key
    uint8_t val[4];
} KeyValuePair;



DecodedModbusPacket decodeModbusPacket(const uint8_t *data, size_t dataLength);
void decodeModbusBodyCommand(DecodedModbusBody *body, DecodedModbusPacket packet);
void decodeModbusBodyData(DecodedModbusBody *body, DecodedModbusPacket packet, uint16_t startAddress);
size_t encodeModbusCommand(uint8_t *buffer, DecodedModbusBody *body);
size_t encodeModbusData(uint8_t *buffer, DecodedModbusBody *body);
bool isInCodeList(int code, const int* codeList);
bool verifyModbusCRC(const uint8_t *data, size_t dataLength, bool isData);
uint16_t CRC16(const uint8_t *data, size_t length);

uint8_t extractRegisterPairs(KeyValuePair *pkv, uint8_t *pd, uint8_t len, uint16_t regAddr, uint8_t bytesPerRegister);

#endif // MODBUS_H
