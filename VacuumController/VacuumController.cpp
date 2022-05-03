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
    pump = _pump{
        .pwm = 0,
        .frequency=0,
        .status=0
    };

    pid = _pid{
        .SetSpeed = 0,            	//定义设定值
        .ActualSpeed = 0,        	//定义实际值
        .err = 0,                	//定义偏差值
        .err_last = 0,            	//定义上一个偏差值
        .Kp = 0,
        .Ki = 0,
        .Kd = 0,            	//定义比例、积分、微分系数
        .voltage = 0,            	//定义电压值（控制执行器的变量）
        .integral = 0,            	//定义积分值
        .umax = 0,					//正饱和值
        .umin = 0,					//负饱和值
        .dead_band = 0,			//死区，单位为百帕
    };

    hp5806_A = HP5806{
        .Tcomp = 0,
        .Pcomp = 0,
    };

    hp5806_B = HP5806{
        .Tcomp = 0,
        .Pcomp = 0,
    };

    relay_A = _relay{
        .status = 0,            	//继电器的状态: 0为关闭，1为导通
        .id = 1,
    };

    relay_B = _relay{
        .status = 0,            	//继电器的状态: 0为关闭，1为导通
        .id = 2,
    };

    time = _time{
        .flag_time = 0,
        .hour = 0,
        .min = 0,
        .sec = 0,
        .setting_sec = 0,
        .remainder_sec = 0,
    };

    system_status = _system_status{
        .current = 0,
    };

    set_value = 0;
    current_pres = 0;
    output_value = 0;
    output_manual = 0;
    pid_mode = 0;
    leak_status = 0;
}

void VacuumController::SendBuffer(uint8_t *ptr,int len)
{
    QByteArray buffer;
    buffer = QByteArray::fromRawData((char*)ptr, len);
    _socket->write(buffer);
    _socket->flush();
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

void VacuumController::SetTimeSec(float value)
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
