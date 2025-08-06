#include "modbus.h"
#include <stdio.h>

// gcc main.c modbus.c -o modbus
// ./modbus

int main() {
    uint8_t modbusData1[] = {0x01, 0x0F, 0x00, 0x10, 0x00, 0x02, 0x01, 0x02, 0x56, 0x78};
    // 包解码接口
    DecodedModbusPacket decodedPacket = decodeModbusPacket(modbusData1, sizeof(modbusData1) / sizeof(modbusData1[0]));

    DecodedModbusBody decodedBody;
    // 数据解码接口
    decodeModbusBodyData(&decodedBody, decodedPacket, MASTER, COIL_ADDR, 0x0010);

    printf("Function: 0x%02X\n", decodedBody.function); // 功能码
    printf("Address Type: 0x%02X\n", decodedBody.addressType); // 地址类型
    printf("Starting Address: 0x%04X\n", decodedBody.startingAddress); // 起始地址
    printf("Quantity: 0x%04X\n", decodedBody.quantity); // 寄存器或者线圈数量
    printf("Byte Count: 0x%02X\n", decodedBody.byteCount); // 字节数

    for (size_t i = 0; i < decodedBody.quantity; ++i) {
        printf("Address: 0x%04X, Value: 0x%04X\n", decodedBody.data[i].address, decodedBody.data[i].value); // 地址和值
    }

    free(decodedBody.data);

    // 0401003000143C5F 
    uint8_t modbusData2[] = {0x04, 0x01, 0x00, 0x30, 0x00, 0x14, 0x3c, 0x5f};
    // 包解码接口
    decodedPacket = decodeModbusPacket(modbusData2, sizeof(modbusData2) / sizeof(modbusData2[0]));

    //DecodedModbusBody decodedBody;
    // 命令解码接口
    decodeModbusBodyCommand(&decodedBody, decodedPacket, MASTER);

    printf("Function: 0x%02X\n", decodedBody.function); // 功能码
    printf("Address Type: 0x%02X\n", decodedBody.addressType); // 地址类型
    printf("Starting Address: 0x%04X\n", decodedBody.startingAddress); // 起始地址
    printf("Quantity: 0x%04X\n", decodedBody.quantity); // 寄存器或者线圈数量
    printf("DeviceAddress: 0x%02X\n", decodedBody.deviceAddress); // 信宿

    // 040103010101AD8B
    uint8_t modbusData3[] = {0x04, 0x01, 0x03, 0x01, 0x01, 0x01, 0xAD, 0x8B};
    // 包解码接口
    decodedPacket = decodeModbusPacket(modbusData3, sizeof(modbusData3) / sizeof(modbusData3[0]));

    // 数据解码接口
    decodeModbusBodyData(&decodedBody, decodedPacket, SLAVE, COIL_ADDR, 0x0030);

    printf("Function: 0x%02X\n", decodedBody.function); // 功能码
    printf("Address Type: 0x%02X\n", decodedBody.addressType); // 地址类型
    printf("Starting Address: 0x%04X\n", decodedBody.startingAddress); // 起始地址
    printf("Quantity: 0x%04X\n", decodedBody.quantity); // 寄存器或者线圈数量
    printf("Byte Count: 0x%02X\n", decodedBody.byteCount); // 字节数

    for (size_t i = 0; i < decodedBody.quantity; ++i) {
        printf("Address: 0x%04X, Value: 0x%04X\n", decodedBody.data[i].address, decodedBody.data[i].value); // 地址和值
    }

    free(decodedBody.data);

    return 0;
}
