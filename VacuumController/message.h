#ifndef MESSAGE_H
#define MESSAGE_H

/* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;

/* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

//指令解析
#define BUFFER_SIZE 2048//指令缓冲区大小，用户可根据自己单片机的容量修改
#define USER_R3 0xA5//帧头
#define USER_RA 0x5A//帧头

#define registor_read 0x81//寄存器读
#define registor_write 0x80//寄存器写
#define variable_read 0x83//变量读
#define variable_write 0x82//变量写
#define curve_write 0x84//实时曲线

void extract_command(void);

#endif // MESSAGE_H
