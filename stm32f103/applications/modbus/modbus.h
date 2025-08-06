// modbus.h
#ifndef MODBUS_H
#define MODBUS_H

#include <stdint.h>
#include <stdlib.h>

// 主从类型
#define MASTER  1
#define SLAVE   2 

// 地址类型
#define COIL_ADDR     1
#define DATA_ADDR     2
#define DEVICE_STATUS 3
#define RUN_ADDR      4
#define PARAM_ADDR    5

// 根据数据类型决定每个地址的字节数
int getAddressSize(uint8_t addressType);

typedef struct {
    uint16_t address; // 地址
    uint16_t value;   // 值
} AddressValuePair;

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
    uint8_t function;    // 功能码
    uint16_t startingAddress;
    uint16_t quantity;
    uint8_t byteCount;
    AddressValuePair *data; // 指向动态分配的地址和值的数组
    size_t count; // 数组中的元素数量
    uint8_t deviceAddress; // 信宿
    uint8_t addressType; // 地址类型
} DecodedModbusBody;

DecodedModbusPacket decodeModbusPacket(const uint8_t *data, size_t dataLength);
void decodeModbusBodyData(DecodedModbusBody *body, DecodedModbusPacket packet, uint8_t role, uint8_t addressType, uint16_t startAddress);
void decodeModbusBodyCommand(DecodedModbusBody *body, DecodedModbusPacket packet, uint8_t role);

AddressValuePair* extractRegisterPairs(const uint8_t *data, uint16_t startingAddress, size_t quantity, size_t byteCount);
AddressValuePair* extractCoilPairs(const uint8_t *data, uint16_t startingAddress, size_t quantity);

#endif // MODBUS_H
