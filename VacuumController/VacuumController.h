#ifndef VACUUMCONTROLLER_H
#define VACUUMCONTROLLER_H

struct _pump{
    float pwm;            	//泵的占空比,范围0%-100%之间的浮点数
    unsigned char status;   //泵的状态: 0为停止，1为运行或准备
    float frequency;        //泵的运行频率，单位为Hz
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
    float Prawsc;
    float Trawsc;
};

struct _relay{
    unsigned char status;            	//继电器的状态: 0为关闭，1为导通
    unsigned char id;					//继电器的序号: 1为继电器1，2为继电器2
};

struct _rgb{
    unsigned char R_status;           //RGB的R状态：0为熄灭，1为开启
    unsigned char G_status;           //RGB的G状态：0为熄灭，1为开启
    unsigned char B_status;           //RGB的B状态：0为熄灭，1为开启
};

struct _time{
    unsigned char flag_time;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    unsigned char setting_sec;
    unsigned char remainder_sec;
};

struct _system_status{
    unsigned char last_2;		//上上次工作状态
    unsigned char last;		  //上次工作状态
    unsigned char current;  //系统工作状态：0-初始化态、1-准备态、2-自动运行态、3-自动暂停态、4-手动运行态、5-手动暂停态、6-急停态、7-设置态
    unsigned char flag_change; //是否更新过状态
};

class VacuumController
{
public:
    VacuumController();
    ~VacuumController();

    struct _pump pump;		//创建一个泵
    struct _pid pid;		//创建一个pid
    struct HP5806 hp5806_A, hp5806_B;	//创建外置与板载传感器
    struct _relay relay_A,relay_B; //创建继电器1
    struct _rgb rgb;//创建RGB灯
    struct _time time;  //时间模块
    struct _system_status system_status;
    float set_value; //设定负压
    float output_value; //输出值
    float output_manual; //手动输出值
    unsigned char pid_mode;			//pid模式：0为积分分离pid，1为模糊pid
    unsigned char leak_status;			//泄漏标志位：0为未泄漏，1为泄漏
    unsigned char wifi_status;			//wifi标志位：0为未连接，1为已连接

private:
    int test;

};

#endif // VACUUMCONTROLLER_H
