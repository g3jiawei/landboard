// modbus.c
#include "modbus.h"
#include <stdio.h>

// 根据数据类型决定每个地址的字节数
int getAddressSize(uint8_t addressType) {
    switch (addressType) {
        case COIL_ADDR:
            return 1; // 命令地址每个地址前进1个bit
        case DATA_ADDR:
        case RUN_ADDR:
        case PARAM_ADDR:
            return 4; // 数据地址、运行地址和参数地址每个地址前进4个字节
        case DEVICE_STATUS:
            return 2; // 设备状态地址前进2个字节
        default:
            return 0; // 未知或不处理的地址类型
    }
}


// 解码函数，将uint8_t数组解码为ModbusPacket结构体
DecodedModbusPacket decodeModbusPacket(const uint8_t *data, size_t dataLength) {
    ModbusPacket packet;

    if (data && dataLength >= 5) { // 确保有足够的数据来提取包头和至少一个字节的包体
        packet.header.address = data[0]; // 解码包头
        packet.body.function = data[1]; // 解码包体的第一个字节，也就是功能码
        packet.body.data = data + 2; // 剩下的部分作为数据，只存储指针和长度
        packet.body.length = dataLength - 4; // 减去包头的1字节，包体的1字节和包尾的2字节
        packet.footer.crc[0] = data[dataLength - 2]; // 解码包尾（CRC），位于数据的最后两个字节
        packet.footer.crc[1] = data[dataLength - 1];
    } else {
        packet = (ModbusPacket){{0}, {0, NULL, 0}, {0, 0}}; // 返回一个空的ModbusPacket结构体
    }

    return decodeModbusPacket_(packet);
}

// 包解码函数实现
DecodedModbusPacket decodeModbusPacket_(ModbusPacket packet) {
    DecodedModbusPacket decodedPacket;
    decodedPacket.body = packet.body;
    decodedPacket.address = packet.header.address;
    return decodedPacket;
}

// 解析地址和值对于写多个寄存器的操作，返回动态分配的数组
AddressValuePair* extractRegisterPairs(const uint8_t *data, uint16_t startingAddress, size_t quantity, size_t byteCount) {
    size_t bytesPerRegister = byteCount / quantity; // 计算每个寄存器的字节数
    AddressValuePair* pairs = (AddressValuePair*)malloc(quantity * sizeof(AddressValuePair));
    if (!pairs) return NULL;

    for (size_t i = 0; i < quantity; ++i) {
        pairs[i].address = startingAddress + i;
        if (bytesPerRegister == 4) { // 处理四字节的寄存器值
            pairs[i].value = (uint32_t)data[i * bytesPerRegister] << 24 |
                             (uint32_t)data[i * bytesPerRegister + 1] << 16 |
                             (uint32_t)data[i * bytesPerRegister + 2] << 8 |
                             data[i * bytesPerRegister + 3];
        } else { // 默认处理为2字节每个寄存器
            pairs[i].value = (uint16_t)data[i * bytesPerRegister] << 8 |
                             data[i * bytesPerRegister + 1];
        }
    }

    return pairs;
}

// 解析地址和值对于写多个线圈的操作，返回动态分配的数组
AddressValuePair* extractCoilPairs(const uint8_t *data, uint16_t startingAddress, size_t quantity) {
    AddressValuePair* pairs = (AddressValuePair*)malloc(quantity * sizeof(AddressValuePair));
    if (!pairs) return NULL;

    for (size_t i = 0; i < quantity; ++i) {
        pairs[i].address = startingAddress + i;
        size_t byteIndex = i / 8;
        size_t bitIndex = i % 8;
        pairs[i].value = (data[byteIndex] & (1 << bitIndex)) ? 1 : 0; // 按位解析每个线圈的状态
    }

    return pairs;
}


void decodeModbusBodyCommand(DecodedModbusBody *body, DecodedModbusPacket packet, uint8_t role) {
    body->function = packet.body.function;
    body->deviceAddress = packet.address;

    if (packet.body.length < 4) {
        printf("Data length error\n");
        return;
    }

    body->startingAddress = (uint16_t)packet.body.data[0] << 8 | packet.body.data[1];
    body->quantity = (uint16_t)packet.body.data[2] << 8 | packet.body.data[3];

    if (role == MASTER) {
        switch (body->function) {
            case 0x01: // 读取多个线圈或离散输入
                body->addressType = COIL_ADDR;
                break;
            case 0x03: // 读取多个保持寄存器或输入寄存器
                body->addressType = DATA_ADDR;
                break;
            default:
                printf("Function code not supported for master\n");
                break;
        }
    } else if (role == SLAVE) {
        switch (body->function) {
            case 0x0F: // 写多个线圈
                body->addressType = COIL_ADDR;
                break;
            case 0x10: // 写多个寄存器
                body->addressType = DATA_ADDR;
                break;
            default:
                printf("Function code not supported for slave\n");
                break;
        }
    } else {
        printf("Role error\n");
    }
}


void decodeModbusBodyData(DecodedModbusBody *body, DecodedModbusPacket packet, uint8_t role, uint8_t addressType, uint16_t startAddress) {
    body->function = packet.body.function;
    body->addressType = addressType;

    // 确保包体长度足够
    if (packet.body.length < 4) {
        printf("Data length error\n");
        return;
    }

    AddressValuePair addressValuePairs[30];  // 假定最多处理30个地址值对
    if (role == MASTER) {
        // 主机模式下的处理逻辑
        switch (body->function) {
            case 0x0F:  // 写多个线圈
                body->startingAddress = (uint16_t)packet.body.data[0] << 8 | packet.body.data[1];
                body->quantity = (uint16_t)packet.body.data[2] << 8 | packet.body.data[3];
                body->byteCount = packet.body.data[4];
                // 假设 extractCoilPairs 动态分配内存并返回指针
                body->data = extractCoilPairs(packet.body.data + 5, body->startingAddress, body->quantity);
                break;
            case 0x10:  // 写多个寄存器
                body->startingAddress = (uint16_t)packet.body.data[0] << 8 | packet.body.data[1];
                body->quantity = (uint16_t)packet.body.data[2] << 8 | packet.body.data[3];
                body->byteCount = packet.body.data[4];
                // 假设 extractRegisterPairs 动态分配内存并返回指针
                body->data = extractRegisterPairs(packet.body.data + 5, body->startingAddress, body->quantity, body->byteCount);
                break;
            case 0x05:  // 写单个线圈
            case 0x06:  // 写单个寄存器
                body->startingAddress = (uint16_t)packet.body.data[0] << 8 | packet.body.data[1];
                // 这里仅处理一个寄存器或线圈，但我们使用动态分配来保持接口的一致性
                body->data = extractRegisterPairs(packet.body.data, body->startingAddress, 1, 2);
                body->count = 1; // 只有一个寄存器或线圈
                break;
            default:
                printf("Function code not supported for master\n");
        }
    } else if (role == SLAVE) {
        // 从机模式下的处理逻辑
        switch (body->function) {
            case 0x01:  // 读取多个线圈或离散输入
                // 通常响应中不需要处理起始地址和数量，因为它们已经在请求中给定
                body->startingAddress = startAddress;
                body->byteCount = packet.body.data[0];  // 第一个字节通常是字节计数
                body->quantity = body->byteCount * 8;  // 假设每个字节表示8个线圈
                // 根据byteCount提取线圈状态
                body->data = extractCoilPairs(packet.body.data + 1, body->startingAddress, body->quantity);
                break;
            case 0x03:  // 读取多个保持寄存器或输入寄存器
                body->byteCount = packet.body.data[0];
                size_t registerCount = body->byteCount / getAddressSize(addressType);
                body->data = extractRegisterPairs(packet.body.data + 1, startAddress, registerCount, body->byteCount);
                body->count = registerCount;
                break;
            case 0x05:  // 写单个线圈
            case 0x06:  // 写单个寄存器
                body->startingAddress = (uint16_t)packet.body.data[0] << 8 | packet.body.data[1];
                // 这里仅处理一个寄存器或线圈，但我们使用动态分配来保持接口的一致性
                body->data = extractRegisterPairs(packet.body.data, body->startingAddress, 1, 2);
                body->count = 1; // 只有一个寄存器或线圈
                break;
            default:
                printf("Function code not supported for slave\n");
        }
    } else {
        printf("Role error\n");
    }
}



