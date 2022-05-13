#ifndef VACUUMCONTROLLER_H
#define VACUUMCONTROLLER_H

#include <QTcpSocket>

struct _pump{
    float pwm;            	//泵的占空比,范围0%-100%之间的浮点数
    float frequency;        //泵的运行频率，单位为Hz
    unsigned char status;   //泵的状态: 0为停止，1为运行或准备
};

struct _pid{
    float SetSpeed;            	//定义设定值
    float ActualSpeed;        	//定义实际值
    float err;                	//定义偏差值
    float err_last;            	//定义上一个偏差值
    float Kp,Ki,Kd;            	//定义比例、积分、微分系数
    float voltage;            	//定义电压值（控制执行器的变量）
    float integral;            	//定义积分值
    float umax;					//正饱和值
    float umin;					//负饱和值
    float dead_band;			//死区，单位为百帕
};

struct HP5806{
    //计算校准输出值
    float Tcomp;
    float Pcomp;
};

struct _relay{
    unsigned char status;            	//继电器的状态: 0为关闭，1为导通
    unsigned char id;					//继电器的序号: 1为继电器1，2为继电器2
};


struct _time{
    unsigned char flag_time;
    unsigned short int hour;
    unsigned char min;
    unsigned char sec;
    short int setting_sec;
    short int remainder_sec;
};

struct _system_status{
    unsigned char current;  //系统工作状态：0-初始化态、1-准备态、2-自动运行态、3-自动暂停态、4-手动运行态、5-手动暂停态、6-急停态、7-设置态
};

class VacuumController
{
public:
    // 所有的指令都不对成员函数进行写，成员函数的所有值默认来自于下位机
    VacuumController();
    ~VacuumController();
    void Init(void);
    void SetValue(float value);
    void SetPWM(float value);
    void SetTimeSec(int value);
    void OpenOrCloseRelay(bool isOpen, int id);
    void ImportConf(void);
    void ExportConf(void);
    void Sync(void);
    void linkTcp(QTcpSocket *socket);
    void Run(int);
    void Stop(void);
    void Know(void);


    struct _pump pump;		//创建一个泵
    struct _pid pid;		//创建一个pid
    struct HP5806 hp5806_A, hp5806_B;	//创建外置与板载传感器
    struct _relay relay_A,relay_B; //创建继电器1
    struct _time time;  //时间模块
    struct _system_status system_status;
    float set_value; //设定负压
    float current_pres;
    float output_value; //输出值
    float output_manual; //手动输出值
    unsigned char pid_mode;			//pid模式：0为积分分离pid，1为模糊pid
    unsigned short int safeEvent_status;			//安全警告
    int WriteData_Enable;
    int File_isClose;
    QString fileName;

private:
    QTcpSocket *_socket;
    void SendBuffer(uint8_t *ptr,int len);

};

extern VacuumController QtSystem;

#endif // VACUUMCONTROLLER_H
