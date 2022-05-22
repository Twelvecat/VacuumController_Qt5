#include "VacuumController.h"
#include "message.h"

VacuumController QtSystem;

VacuumController::VacuumController()
{
    Init();
}

VacuumController::~VacuumController()
{

}

void VacuumController::linkTcp(QTcpSocket *socket)
{
    _socket = socket;
}

void VacuumController::Init(void)
{
    pump.pwm = 0;
    pump.frequency=0;
    pump.status=0;


    pid.SetSpeed = 0;            	//定义设定值
    pid.ActualSpeed = 0;        	//定义实际值
    pid.err = 0;                	//定义偏差值
    pid.err_last = 0;            	//定义上一个偏差值
    pid.Kp = 0;
    pid.Ki = 0;
    pid.Kd = 0;            	//定义比例、积分、微分系数
    pid.voltage = 0;            	//定义电压值（控制执行器的变量）
    pid.integral = 0;            	//定义积分值
    pid.umax = 0;					//正饱和值
    pid.umin = 0;					//负饱和值
    pid.dead_band = 0;			//死区，单位为百帕


    hp5806_A.Tcomp = 0;
    hp5806_A.Pcomp = 0;

    hp5806_B.Tcomp = 0;
    hp5806_B.Pcomp = 0;

    relay_A.status = 0;            	//继电器的状态: 0为关闭，1为导通
    relay_A.id = 1;

    relay_B.status = 0;            	//继电器的状态: 0为关闭，1为导通
    relay_B.id = 2;
    time.flag_time = 0;
    time.hour = 0;
    time.min = 0;
    time.sec = 0;
    time.setting_sec = 0;
    time.remainder_sec = 0;

    system_status.current = 1;
 

    set_value = 0;
    current_pres = 0;
    output_value = 0;
    output_manual = 0;
    pid_mode = 0;
    control_time = 0;
    safeEvent_status  = 0;
    WriteData_Enable = 0;
    File_isClose = 1;
    fileName = "";
}

void VacuumController::SendBuffer(uint8_t *ptr,int len)
{
    QByteArray buffer;
    buffer = QByteArray::fromRawData((char*)ptr, len);
    _socket->write(buffer);
    _socket->flush();

//确认发送信息
//#define SEND_INFO
#ifdef SEND_INFO
    qDebug() << "Message Success!";
    QString strTmp;
    for(int n = 0; n<len; n++)
    {
        strTmp +=  QString().sprintf("%02X", (unsigned char)buffer[n]);
        strTmp += " ";
    }
    qDebug() << strTmp;
#endif
}

void VacuumController::SetValue(float value)
{
    if(value>700)value = 700;
    else if(value <0)value = 0;
    uint16_t temp = value*10;
    TxBuff_8309[4] = (uint8_t)(UIaddr_set_pres >> 8);
    TxBuff_8309[5] = (uint8_t)(UIaddr_set_pres&0x00FF);
    TxBuff_8309[7] = (uint8_t)(temp >> 8);
    TxBuff_8309[8] = (uint8_t)(temp&0x00FF);

    SendBuffer(TxBuff_8309,9);
}

void VacuumController::SetPWM(float value)
{
    if(value>100)value = 100;
    else if(value <0)value = 0;
    uint16_t temp = (uint16_t)value;
    TxBuff_8309[4] = (uint8_t)(UIaddr_manual_pwm >> 8);
    TxBuff_8309[5] = (uint8_t)(UIaddr_manual_pwm&0x00FF);
    TxBuff_8309[7] = (uint8_t)(temp >> 8);
    TxBuff_8309[8] = (uint8_t)(temp&0x00FF);

    SendBuffer(TxBuff_8309,9);
}

void VacuumController::SetTimeSec(int value)
{
    if(value>32767)value = 32767;
    else if(value < -1)value = -1;
    uint16_t temp = (uint16_t)value;
    TxBuff_8309[4] = (uint8_t)(UIaddr_setting_sec >> 8);
    TxBuff_8309[5] = (uint8_t)(UIaddr_setting_sec&0x00FF);
    TxBuff_8309[7] = (uint8_t)(temp >> 8);
    TxBuff_8309[8] = (uint8_t)(temp&0x00FF);

    SendBuffer(TxBuff_8309,9);
}

void VacuumController::OpenOrCloseRelay(bool isOpen, int id)
{
    uint16_t temp = 0x0000;
    if(id == 1)
    {
        temp = (uint16_t)((((uint8_t)isOpen & 0xFF) << 1) | (relay_B.status & 0xFF) );
    }
    else if (id == 2)
    {
        temp = (uint16_t)(((relay_A.status & 0xFF) << 1) | ((uint8_t)isOpen & 0xFF) );
    }
    TxBuff_8309[4] = (uint8_t)(UIaddr_relay_status >> 8);
    TxBuff_8309[5] = (uint8_t)(UIaddr_relay_status&0x00FF);
    TxBuff_8309[7] = (uint8_t)(temp >> 8);
    TxBuff_8309[8] = (uint8_t)(temp&0x00FF);

    SendBuffer(TxBuff_8309,9);
}

void VacuumController::Run(int flag)
{
    uint16_t addr = 0x0000;
    uint16_t data = 0x0000;
    if(system_status.current == 1){
        if(flag == 1){//自动
            addr = UIaddr_auto;
            data = 0x0101;
        }
        else if(flag == 0)//手动
        {
            addr = UIaddr_manual;
            data = 0x0101;
        }
    }
    else if(system_status.current == 2)
    {
        addr = UIaddr_auto;
        data = 0x0102;
    }
    else if(system_status.current == 3)
    {
        addr = UIaddr_auto;
        data = 0x0103;
    }
    else if(system_status.current == 4)
    {
        addr = UIaddr_manual;
        data = 0x0102;
    }
    else if(system_status.current == 5)
    {
        addr = UIaddr_manual;
        data = 0x0103;
    }
    TxBuff_8309[4] = (uint8_t)(addr >> 8);
    TxBuff_8309[5] = (uint8_t)(addr&0x00FF);
    TxBuff_8309[7] = (uint8_t)(data >> 8);
    TxBuff_8309[8] = (uint8_t)(data&0x00FF);

    SendBuffer(TxBuff_8309,9);
}

void VacuumController::Stop(void)
{
    uint16_t temp = 0x0101;
    TxBuff_8309[4] = (uint8_t)(UIaddr_stop >> 8);
    TxBuff_8309[5] = (uint8_t)(UIaddr_stop&0x00FF);
    TxBuff_8309[7] = (uint8_t)(temp >> 8);
    TxBuff_8309[8] = (uint8_t)(temp&0x00FF);

    SendBuffer(TxBuff_8309,9);
}

void VacuumController::Know(void)
{
    uint16_t temp = 0x0101;
    TxBuff_8309[4] = (uint8_t)(UIaddr_warring >> 8);
    TxBuff_8309[5] = (uint8_t)(UIaddr_warring&0x00FF);
    TxBuff_8309[7] = (uint8_t)(temp >> 8);
    TxBuff_8309[8] = (uint8_t)(temp&0x00FF);

    SendBuffer(TxBuff_8309,9);
}

void VacuumController:: ImportConf(void)
{
//再说吧
}

void VacuumController:: ExportConf(void)
{
//再说吧
}

void VacuumController:: Sync(void)
{
//再说吧
}
