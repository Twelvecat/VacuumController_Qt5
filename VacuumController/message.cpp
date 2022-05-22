#include "message.h"
#include <QDebug>

uint16_t StartNum=0,TalNum=0;
uint8_t CommBuff[BUFFER_SIZE];//定义指令缓冲区
uint8_t TxBuff_8309[9] = {USER_R3, USER_RA, 0x06, variable_read, 0x00, 0x00, 0x01, 0x00, 0x00};

void extract_command(VacuumController *QtSystem)
{
    uint16_t i,CurNum,tem_TalNum;
    uint8_t CmdBuf[256];
    uint16_t nowbuffer,len;
    len=StartNum;
    tem_TalNum=TalNum;
    if(tem_TalNum==len)
        return;
    if(CommBuff[StartNum]!=USER_R3)
    {
        StartNum++;
        if(StartNum==BUFFER_SIZE)
            StartNum=0;
        return;
    }
    if(tem_TalNum>len)
        nowbuffer=tem_TalNum-len;
    else
        nowbuffer=tem_TalNum+BUFFER_SIZE-len;
    if(nowbuffer<5)
        return;
    CurNum=StartNum+2;
    if(CurNum>BUFFER_SIZE-1)
        CurNum-=BUFFER_SIZE;
    len=CommBuff[CurNum]+3;
    if(nowbuffer<len)
        return;
    i=0;
    CurNum=StartNum;
    while(1)
    {
        CmdBuf[i++]=CommBuff[CurNum++];
        if(CurNum==BUFFER_SIZE)
            CurNum=0;
        if(i==4)
        {
            if(CmdBuf[0]!=USER_R3||CmdBuf[1]!=USER_RA)
            {
                StartNum=CurNum;
                return;
            }
            len=CmdBuf[2];
        }
        else if(i>4)
        {
            if(i==len+3)//
            {
                StartNum=CurNum;
                break;
            }
            else if(i>255)//
            {
                StartNum=CurNum;
                return;
            }
            else if(CurNum==tem_TalNum)
                return;
        }
    }

    //确认指令信息
//#define MESSAGE_INFO
#ifdef MESSAGE_INFO
    qDebug() << "Message Success!";
    QString strTmp;
    for(int n = 0; n<len+3; n++)
    {
        strTmp +=  QString().sprintf("%02X", (unsigned char)CmdBuf[n]);
        strTmp += " ";
    }
    qDebug() << strTmp;
#endif

    //现在解析指令CmdBuf保存一整条指令

    switch(CmdBuf[3])
    {
        case registor_write:
            break;
        case registor_read:
            //处理
            break;
        case variable_write:
        {
            TOUCH_deal_82command(QtSystem,CmdBuf);
            break;
        }
        case variable_read:
            //处理
            break;
        case curve_write:
            //处理
            break;
        default:
            //命令无效,删除
            break;
    }
    return;
}


void TOUCH_deal_82command(VacuumController *QtSystem, uint8_t *p_Cmdbuf)
{
    uint8_t data_len = p_Cmdbuf[2];
    if(data_len != 5) return;
    uint16_t command_adds = ((uint16_t)p_Cmdbuf[4] << 8) | ((uint16_t)p_Cmdbuf[5]); //指令地址
    uint16_t temp_data = ((uint16_t)p_Cmdbuf[6] << 8) | ((uint16_t)p_Cmdbuf[7]);

    if(QtSystem->system_status.current == 6)
    {
        if(System_status != command_adds)return;
    }

    if(UIaddr_hp5806_B_pres == command_adds)
    {
        QtSystem->hp5806_B.Pcomp = (float)(temp_data/10.0);
    }
    else if(UIaddr_hp5806_A_pres == command_adds)
    {
        QtSystem->hp5806_A.Pcomp = (float)(temp_data/10.0);
    }
    else if(UIaddr_hp5806_B_temp == command_adds)
    {
        QtSystem->hp5806_B.Tcomp = (float)(temp_data/10.0);
    }
    else if(UIaddr_hp5806_A_temp == command_adds)
    {
        QtSystem->hp5806_A.Tcomp = (float)(temp_data/10.0);
    }
    else if(UIaddr_set_pres == command_adds)
    {
        QtSystem->set_value = (float)(temp_data/10.0);
    }
    else if(UIaddr_current_pres == command_adds)
    {
        QtSystem->current_pres = (float)(temp_data/10.0);
    }
    else if(UIaddr_relay_status == command_adds)
    {
        QtSystem->relay_A.status = (temp_data & 0x0002) > 1;
        QtSystem->relay_B.status = (temp_data & 0x0001);
    }
    else if(UIaddr_hour == command_adds)
    {
        QtSystem->time.hour = temp_data;
    }
    else if(UIaddr_min == command_adds)
    {
        QtSystem->time.min = temp_data;
    }
    else if(UIaddr_sec == command_adds)
    {
        QtSystem->time.sec = temp_data;
    }
    else if(UIaddr_setting_sec == command_adds)
    {
        QtSystem->time.setting_sec = temp_data;
    }
    else if(UIaddr_remainder_sec == command_adds)
    {
        QtSystem->time.remainder_sec = temp_data;
    }
    else if(UIaddr_pump_status == command_adds)
    {
        QtSystem->pump.status = temp_data;
    }
    else if(System_status == command_adds)
    {
        QtSystem->system_status.current = p_Cmdbuf[7];
    }
    else if(Output_status == command_adds)
    {
        QtSystem->output_value = temp_data/40.0;
    }
    else if(UIaddr_manual_pwm == command_adds)
    {
        QtSystem->output_manual = temp_data;
    }
    else if(UIaddr_pid_mode == command_adds)
    {
        QtSystem->pid_mode = p_Cmdbuf[7];
        if(QtSystem->pid_mode==0)printf("pid_mode:PID\n");
        else if(QtSystem->pid_mode==1)printf("pid_mode:fuzzyPID\n");
        else printf("pid_mode:ERROR\n");
    }
    else if(UIaddr_contorl_time == command_adds)
    {
        QtSystem->control_time = p_Cmdbuf[7];
        printf("control_time:%dms\n",QtSystem->control_time*200);
    }
    else if(UIaddr_pid_p == command_adds)
    {
        QtSystem->pid.Kp = ((int16_t)temp_data)/1000.0;
        printf("pid_Kp:%.3f\n",QtSystem->pid.Kp);
    }
    else if(UIaddr_pid_i == command_adds)
    {
        QtSystem->pid.Ki = ((int16_t)temp_data)/1000.0;
        printf("pid_Ki:%.3f\n",QtSystem->pid.Ki);
    }
    else if(UIaddr_pid_d == command_adds)
    {
        QtSystem->pid.Kd = ((int16_t)temp_data)/1000.0;
        printf("pid_Kd:%.3f\n",QtSystem->pid.Kd);
    }
    else if(UIaddr_pid_delta_k == command_adds)
    {
        QtSystem->deltak = ((int16_t)temp_data)/1000.0;
        printf("pid_Delta:%.3f\n",QtSystem->deltak);
    }
    else if(UIaddr_warring == command_adds)
    {
        /*
        0x01: 温度上限 P：1
        0x02: 温度下限 P：1
        0x03: 气压上限 P：4
        0x04: 气压下限 P：5
        0x05: 管道泄漏 P：2
        0x06: 设备初始化未通过 P：3
        */
        QtSystem->safeEvent_status = temp_data;
    }
}
