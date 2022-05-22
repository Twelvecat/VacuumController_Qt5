#ifndef MESSAGE_H
#define MESSAGE_H
#include "VacuumController.h"
#include <QMessageBox>

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

// 仪表盘
#define UIaddr_hp5806_B_pres 0x0080
#define UIaddr_hp5806_A_pres 0x0084
#define UIaddr_hp5806_B_temp 0x0088
#define UIaddr_hp5806_A_temp 0x008C

// 负压情况
#define UIaddr_set_pres 0x000F
#define UIaddr_current_pres 0x000E

// 继电器
#define UIaddr_relay_status 0x003A

// 计时模块
#define UIaddr_hour 0x000B
#define UIaddr_min 0x000C
#define UIaddr_sec 0x000D
#define UIaddr_setting_sec 0x0037
#define UIaddr_remainder_sec 0x0038

// 启停终模块 - 暂未启用，此部分使用状态切换指令
#define UIaddr_auto 0x0000
#define UIaddr_auto_run 0x0001
#define UIaddr_auto_pause 0x0002
#define UIaddr_auto_continue 0x0003
#define UIaddr_manual 0x0024
#define UIaddr_manual_run 0x0001
#define UIaddr_manual_pause 0x0002
#define UIaddr_manual_continue 0x0003
#define UIaddr_stop 0x0022

// 状态，伪造，注意冲突
#define System_status 0x003D
#define Output_status 0x013D

// 变量图标 - WiFi暂未启用，emm没啥意义
#define UIaddr_pump_status 0x003E
#define UIaddr_pump_off 0x0000
#define UIaddr_pump_on 0x0001
#define UIaddr_pump_stop 0x0002
#define UIaddr_wifi_status 0x003D
#define UIaddr_wifi_off 0x0000
#define UIaddr_wifi_on 0x0001

// 手动PWM - 单独控制
#define UIaddr_manual_pwm 0x0039

// 设置板块 - 上电复位可以考虑后续设计
#define UIaddr_reset 0x0023

// 警告板块 - 应该是给下位机发送
#define UIaddr_warring 0x0036

//控制参数
#define UIaddr_pid_mode 0x0090
#define UIaddr_pid_p 0x0091
#define UIaddr_pid_i 0x0092
#define UIaddr_pid_d 0x0093
#define UIaddr_pid_delta_k 0x0094
#define UIaddr_pid_updata 0x0095
#define UIaddr_contorl_time 0x0096

//暂未启用
#define UIaddr_pump_freq 0x0094
#define UIaddr_pump_pwm 0x0098
#define UIaddr_leak_status 0x00B8
#define UIaddr_system_status 0x00C0
#define UIaddr_set_value 0x0826


extern uint16_t StartNum,TalNum;
extern uint8_t CommBuff[BUFFER_SIZE];//定义指令缓冲区
extern uint8_t TxBuff_8309[9];

void extract_command(VacuumController *QtSystem);
void TOUCH_deal_82command(VacuumController *QtSystem, uint8_t *p_Cmdbuf);
#endif // MESSAGE_H
