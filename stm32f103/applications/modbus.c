
#include "modbus.h"
#include <stdio.h>
#include <string.h>

FunctionCodes g_codes;

void readConfig(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open config file.\n");
        exit(1);
    }
    char line[256];
    int index;
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line + 6, ", \n");  // 注意这里增加了对换行符的处理
        index = 0;
        if (strncmp(line, "Type01:", 6) == 0) {
            while (token != NULL && index < MAX_CODE_COUNT) {
                g_codes.type01Codes[index++] = strtol(token, NULL, 16);  // 使用16进制解析
                token = strtok(NULL, ", ");
            }
        } else if (strncmp(line, "Type03:", 6) == 0) {
            while (token != NULL && index < MAX_CODE_COUNT) {
                g_codes.type03Codes[index++] = strtol(token, NULL, 16);  // 使用16进制解析
                token = strtok(NULL, ", ");
            }
        } else if (strncmp(line, "Type05:", 6) == 0) {
            while (token != NULL && index < MAX_CODE_COUNT) {
                g_codes.type05Codes[index++] = strtol(token, NULL, 16);  // 使用16进制解析
                token = strtok(NULL, ", ");
            }
        } else if (strncmp(line, "Type06:", 6) == 0) {
            while (token != NULL && index < MAX_CODE_COUNT) {
                g_codes.type06Codes[index++] = strtol(token, NULL, 16);  // 使用16进制解析
                token = strtok(NULL, ", ");
            }
        } else if (strncmp(line, "Type0F:", 6) == 0) {
            while (token != NULL && index < MAX_CODE_COUNT) {
                g_codes.type0FCodes[index++] = strtol(token, NULL, 16);  // 使用16进制解析
                token = strtok(NULL, ", ");
            }
        } else if (strncmp(line, "Type10:", 6) == 0) {
            while (token != NULL && index < MAX_CODE_COUNT) {
                g_codes.type10Codes[index++] = strtol(token, NULL, 16);  // 使用16进制解析
                token = strtok(NULL, ", ");
            }
        }
    }
    fclose(file);
}

bool isInCodeList(int code, const int* codeList) {
    for (int i = 0; i < MAX_CODE_COUNT; i++) {
        if (codeList[i] == 0) break; // Assuming zero is not a valid function code
        if (codeList[i] == code) return true;
    }
    return false;
}


// 包解码函数实现
DecodedModbusPacket decodeModbusPacket_(ModbusPacket packet) {
    DecodedModbusPacket decodedPacket;
    decodedPacket.body = packet.body;
    decodedPacket.address = packet.header.address;
    return decodedPacket;
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

void decodeModbusBodyCommand(DecodedModbusBody *body, DecodedModbusPacket packet) {
    body->function = packet.body.function;
    body->deviceAddress = packet.address;

    if (packet.body.length < 4) {
        printf("Data length error\n");
        return;
    }

    body->startingAddress = (uint16_t)packet.body.data[0] << 8 | packet.body.data[1];

    switch (body->function) {
        case 0x01: // 读取多个线圈或离散输入
        case 0x03: // 读取多个保持寄存器或输入寄存器
            body->quantity = (uint16_t)packet.body.data[2] << 8 | packet.body.data[3];
            break;
        case 0x05:  // 写单个线圈
        case 0x06:  // 写单个寄存器
            body->count = (uint8_t)2;
            body->data = (uint8_t *)rt_malloc(body->count * sizeof(uint8_t)); // Allocate memory
            rt_memcpy(body->data, packet.body.data + 2, body->count); // Copy from packet
            break;
        case 0x0F: // 写多个线圈
        case 0x10: // 写多个寄存器
            body->quantity = (uint16_t)packet.body.data[2] << 8 | packet.body.data[3];
            body->byteCount = packet.body.data[4];
            body->count = body->byteCount;  // 字节数
            body->data = (uint8_t *)rt_malloc(body->count * sizeof(uint8_t));  // Allocate memory 
            rt_memcpy(body->data, packet.body.data + 5, body->count);  // Copy from packet
            break;
        default:
            printf("Function code not supported\n");
            break;
    }

}

void decodeModbusBodyData(DecodedModbusBody *body, DecodedModbusPacket packet, uint16_t startAddress) {
    body->function = packet.body.function;

    // 确保包体长度足够
    if (packet.body.length < 2) {
        printf("Data length error\n");
        return;
    }

    switch (body->function) {
        case 0x01: // 读取多个线圈或离散输入
        case 0x03: // 读取多个保持寄存器或输入寄存器
            body->startingAddress = startAddress;
            body->byteCount = (uint8_t)packet.body.data[0];
            body->count = body->byteCount;
            body->data = (uint8_t *)rt_malloc(body->count * sizeof(uint8_t)); // Allocate memory
            rt_memcpy(body->data, packet.body.data + 1, body->count); // Copy from packet
            break;
        case 0x05:  // 写单个线圈
        case 0x06:  // 写单个寄存器
            body->startingAddress = (uint16_t)packet.body.data[0] << 8 | packet.body.data[1];
            body->count = (uint8_t)2;
            body->data = (uint8_t *)rt_malloc(body->count * sizeof(uint8_t)); // Allocate memory
            rt_memcpy(body->data, packet.body.data + 2, body->count); // Copy from packet
            break;
        case 0x0F: // 写多个线圈
        case 0x10: // 写多个寄存器
            body->startingAddress = (uint16_t)packet.body.data[0] << 8 | packet.body.data[1];
            body->quantity = (uint16_t)packet.body.data[2] << 8 | packet.body.data[3];
            break;
        default:
            printf("Function code not supported\n");
            break;
    }
}




// 函数定义，返回Modbus数据包的预期长度
size_t getModbusCommandLength(const uint8_t *data, size_t dataLength) {
    uint8_t byteCount;
    uint8_t functionCode;
    
    if (data == NULL || dataLength < 2) { // 需要至少有地址和功能码
        return 0; // 数据不足以判断
    }
    
    functionCode = data[1]; // 功能码位于第二个字节

    switch (functionCode) {
        case 0x01: // 读取多个线圈或离散输入
        case 0x03: // 读取多个保持寄存器或输入寄存器
        case 0x05: // 写单个线圈
        case 0x06: // 写单个寄存器
            return 8; // 地址(1) + 功能码(1) + 起始地址(2) + 数量(2) + CRC(2)

        case 0x0F: // 写多个线圈
        case 0x10: // 写多个寄存器
            if (dataLength < 7) {
                return 0; // 数据不足以判断字节数
            }
            byteCount = data[6]; // 字节数位于第7个字节
            return 7 + byteCount + 2; // 地址(1) + 功能码(1) + 起始地址(2) + 数量(2) + 字节数(1) + 数据(byteCount) + CRC(2)

        default:
            printf("Unsupported function code: 0x%02X\n", functionCode);
            return 0; // 未知功能码
    }
}

// 函数定义，返回基于响应的Modbus数据包的预期长度
size_t getModbusDataLength(const uint8_t *data, size_t dataLength) {
    uint8_t byteCount,functionCode;
    
    if (data == NULL || dataLength < 2) { // 至少需要有地址和功能码
        return 0; // 数据不足以判断
    }

    functionCode = data[1]; // 功能码位于第二个字节

    switch (functionCode) {
        case 0x01: // 读取多个线圈
        case 0x03: // 读取多个保持寄存器
            if (dataLength < 3) {
                return 0; // 数据不足以判断字节数
            }
            byteCount = data[2]; // 字节数位于第三个字节
            return 3 + byteCount + 2; // 地址(1) + 功能码(1) + 字节数(1) + 数据(byteCount) + CRC(2)

        case 0x05: // 写单个线圈
        case 0x06: // 写单个寄存器
            return 8; // 地址(1) + 功能码(1) + 起始地址(2) + 数量(2) + CRC(2)

        case 0x0F: // 写多个线圈
        case 0x10: // 写多个寄存器
            return 8; // 地址(1) + 功能码(1) + 起始地址(2) + 数量(2) + CRC(2)

        default:
            printf("Unsupported function code: 0x%02X\n", functionCode);
            return 0; // 未知功能码
    }
}

// 函数定义，返回基于响应的Modbus数据包的预期长度
size_t geteb95Length(const uint8_t *data, size_t dataLength) {
    
    if (data == NULL || dataLength < 3) { // 至少需要有地址和功能码
        return 0; // 数据不足以判断
    }
    return data[2]+11+5; // 未知功能码
}



// 计算 Modbus RTU 的 CRC-16
uint16_t CRC16(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFF; // 初始值
    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i]; // 将字节数据异或进CRC寄存器
        for (int j = 0; j < 8; j++) { // 处理每个位
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001; // 如果LSB为1，进行右移并异或多项式
            } else {
                crc >>= 1; // 否则只进行右移
            }
        }
    }
    return crc;
}

// 验证 Modbus 数据包的 CRC
bool verifyModbusCRC(const uint8_t *data, size_t dataLength, bool isData) {
    if (data == NULL || dataLength < 5) { // 至少需要有地址、功能码和CRC
        return false; // 数据不足以判断
    }

    size_t expectedLength = isData ? getModbusDataLength(data, dataLength) : getModbusCommandLength(data, dataLength);
    if (expectedLength == 0 || expectedLength > dataLength) {
        return false; // 长度错误或数据不完整
    }

    // 提取CRC值（数据包的最后两字节）
    uint16_t receivedCRC = (uint16_t)(data[expectedLength - 2] | data[expectedLength - 1] << 8);

    // 计算CRC（除去最后两个字节）
    uint16_t calculatedCRC = CRC16(data, expectedLength - 2);
    //printf("CRC16: 0x%04X\n", calculatedCRC);


    // 比较计算的CRC和接收的CRC
    return (receivedCRC == calculatedCRC);
}

size_t encodeModbusCommand(uint8_t *buffer, DecodedModbusBody *body) {
    size_t index = 0;
    buffer[index++] = 0x01;
    buffer[index++] = body->function;

//    if (isInCodeList(body->function, g_codes.type01Codes) || isInCodeList(body->function, g_codes.type03Codes)) {
    if((body->function == 0x01) | (body->function == 0x03)){
        // Type1
        buffer[index++] = (uint8_t)(body->startingAddress >> 8);
        buffer[index++] = (uint8_t)(body->startingAddress & 0xFF);
        buffer[index++] = (uint8_t)(body->quantity >> 8);
        buffer[index++] = (uint8_t)(body->quantity & 0xFF);
//    } else if (isInCodeList(body->function, g_codes.type05Codes) || isInCodeList(body->function, g_codes.type06Codes)) {
     } else if ((body->function == 0x05) | (body->function == 0x06)) {
        // Type2 需要起始地址和数据
        buffer[index++] = (uint8_t)(body->startingAddress >> 8);
        buffer[index++] = (uint8_t)(body->startingAddress & 0xFF);
        rt_memcpy(&buffer[index], body->data, body->count);
        index += body->count;
//    } else if (isInCodeList(body->function, g_codes.type0FCodes) || isInCodeList(body->function, g_codes.type10Codes)) {
    } else if((body->function == 0x00f) | (body->function == 0x10)){
        // Type3 需要起始地址、字节数和数据
        buffer[index++] = (uint8_t)(body->startingAddress >> 8);
        buffer[index++] = (uint8_t)(body->startingAddress & 0xFF);
        buffer[index++] = (uint8_t)(body->quantity >> 8);
        buffer[index++] = (uint8_t)(body->quantity & 0xFF);
        buffer[index++] = (uint8_t)(body->byteCount);
        rt_memcpy(&buffer[index], body->data, body->count);
        index += body->count;
    } else {
        printf("Function code wrong\n");
    }

    uint16_t calculatedCRC = CRC16(buffer, index);
    buffer[index++] = (uint8_t)(calculatedCRC & 0xFF);
    buffer[index++] = (uint8_t)(calculatedCRC >> 8);
    
    return index;
}

size_t encodeModbusData(uint8_t *buffer, DecodedModbusBody *body) {
    size_t index = 0;
    buffer[index++] = body->deviceAddress;
    buffer[index++] = body->function;

//    if (isInCodeList(body->function, g_codes.type01Codes) || isInCodeList(body->function, g_codes.type03Codes)) {
    if((body->function == 0x01) | (body->function == 0x03)){
        buffer[index++] = (uint8_t)(body->byteCount);
        rt_memcpy(&buffer[index], body->data, body->byteCount);
        index += body->byteCount;
//    } else if (isInCodeList(body->function, g_codes.type05Codes) || isInCodeList(body->function, g_codes.type06Codes)) {
    } else if ((body->function == 0x05) | (body->function == 0x06)) {
        // Type2 
        buffer[index++] = (uint8_t)(body->startingAddress >> 8);
        buffer[index++] = (uint8_t)(body->startingAddress & 0xFF);
        rt_memcpy(&buffer[index], body->data, body->count);
        index += body->count;
//    } else if  (isInCodeList(body->function, g_codes.type0FCodes) || isInCodeList(body->function, g_codes.type10Codes)) {
            } else if((body->function == 0x00f) | (body->function == 0x10)){
        // Type3 需要起始地址、字节数和数据
        buffer[index++] = (uint8_t)(body->startingAddress >> 8);
        buffer[index++] = (uint8_t)(body->startingAddress & 0xFF);
        buffer[index++] = (uint8_t)(body->quantity >> 8);
        buffer[index++] = (uint8_t)(body->quantity & 0xFF);
    } else {
                        rt_kprintf("Function code wrong\n");
    }

    uint16_t calculatedCRC = CRC16(buffer, index);
    buffer[index++] = (uint8_t)(calculatedCRC & 0xFF);
    buffer[index++] = (uint8_t)(calculatedCRC >> 8);

    return index;
}

// 寄存器地址转为字符串，用于键值；
uint8_t reg2Key(char *pk, uint16_t reg){
    uint8_t ch;
    ch = reg/10000;
    *pk = 0x30+ch;
    reg -= ch * 10000;
    ch = reg/1000;
    *(pk+1) = 0x30+ch;
    reg -= ch * 1000;
    ch = reg/100;
    *(pk+2) = 0x30+ch;
    reg -= ch * 100;
    ch = reg/10;
    *(pk+3) = 0x30+ch;
    reg -= ch * 10;
    *(pk+4) = 0x30+reg;
    *(pk+5) = 0;

    return 0;
}

uint8_t extractRegisterPairs(KeyValuePair *pkv, uint8_t *pd, uint8_t len, uint16_t regAddr, uint8_t bytesPerRegister){
    uint8_t num, i, j;

    num = len/bytesPerRegister;

    for(i=0; i<num; i++){
        reg2Key((pkv+i)->keyChar, regAddr+(i*(bytesPerRegister>>1)));
        for(j=0; j<bytesPerRegister; j++){
            *((pkv+i)->val+j) = *(pd+(j+i*bytesPerRegister));
        }
    }

    return num;
}

