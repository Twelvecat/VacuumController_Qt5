#include "message.h"
#include <QDebug>

uint16_t StartNum=0,TalNum=0;
uint8_t CommBuff[BUFFER_SIZE];//定义指令缓冲区

void extract_command(void)
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
#define MESSAGE_INFO
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
             qDebug() << "处理成功";//处理
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
