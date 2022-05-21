#include "debugpid.h"
#include "ui_debugpid.h"
#include "message.h"

DebugPID::DebugPID(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugPID)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QString::fromLocal8Bit("PID调试器"));

    ui->label_mode->setStyleSheet("color: #EAEBEB;");
    ui->label_kp->setStyleSheet("color: #EAEBEB;");
    ui->label_ki->setStyleSheet("color: #EAEBEB;");
    ui->label_kd->setStyleSheet("color: #EAEBEB;");
    ui->label_deltak->setStyleSheet("color: #EAEBEB;");
    ui->lineEdit_mode->setStyleSheet("\
        QLineEdit{border-radius: 5px;background: #464C59;color:#EAEBEB;}\
    ");
    ui->lineEdit_kp->setStyleSheet("\
        QLineEdit{border-radius: 5px;background: #464C59;color:#EAEBEB;}\
    ");
    ui->lineEdit_ki->setStyleSheet("\
        QLineEdit{border-radius: 5px;background: #464C59;color:#EAEBEB;}\
    ");
    ui->lineEdit_kd->setStyleSheet("\
        QLineEdit{border-radius: 5px;background: #464C59;color:#EAEBEB;}\
    ");
    ui->lineEdit_deltak->setStyleSheet("\
        QLineEdit{border-radius: 5px;background: #464C59;color:#EAEBEB;}\
    ");
   ui->pushButton_Upload->setStyleSheet("\
       QPushButton{color:#EAEBEB;background-color:#282c34;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
       QPushButton::hover{color:#282c34;background-color:#EAEBEB;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
       QPushButton::pressed{color:#EAEBEB;background-color:#464C59;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
   ");
   ui->pushButton_Download->setStyleSheet("\
       QPushButton{color:#EAEBEB;background-color:#282c34;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
       QPushButton::hover{color:#282c34;background-color:#EAEBEB;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
       QPushButton::pressed{color:#EAEBEB;background-color:#464C59;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
   ");
    ui->pushButton_flash->setStyleSheet("\
        QPushButton{color:#EAEBEB;background-color:#282c34;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
        QPushButton::hover{color:#282c34;background-color:#EAEBEB;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
        QPushButton::pressed{color:#EAEBEB;background-color:#464C59;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
    ");
    ui->groupBox->setStyleSheet("QGroupBox {border:1px solid #EAEBEB;color: #EAEBEB;margin-top:6px;border-radius: 10px;}QGroupBox::title { subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");

    pidtimer = new QTimer(this);
    connect(pidtimer, SIGNAL (timeout(void)), this, SLOT(showpid(void)));
}

DebugPID::~DebugPID()
{
    delete ui;
}

void DebugPID::linkTcp(QTcpSocket *socket)
{
    pid_socket = socket;
}

void DebugPID::SendBuffer(uint8_t *ptr,int len)
{
    QByteArray buffer;
    buffer = QByteArray::fromRawData((char*)ptr, len);
    pid_socket->write(buffer);
    pid_socket->flush();
}

void DebugPID::sendCmd(uint16_t addr,float value, int K)
{
    int16_t temp = value*K;
    TxBuff_8309[4] = (uint8_t)(addr >> 8);
    TxBuff_8309[5] = (uint8_t)(addr&0x00FF);
    TxBuff_8309[7] = (uint8_t)(temp >> 8);
    TxBuff_8309[8] = (uint8_t)(temp&0x00FF);
    SendBuffer(TxBuff_8309,9);
}

void DebugPID::Delay_MSec(unsigned int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));  //创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();  //事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void DebugPID::showpid()
{
    static int count_time = 0;
    count_time++;
    if (count_time > 4){
        pidtimer->stop();
        count_time = 0;
    }
    if(!ui->lineEdit_mode->hasFocus()){
        QString mode = QString("%1").arg(QtSystem.pid_mode);
        ui->lineEdit_mode->setText(mode);
    }
    if(!ui->lineEdit_kp->hasFocus()){
        QString kp = QString("%1").arg(QtSystem.pid.Kp);
        ui->lineEdit_kp->setText(kp);
    }
    if(!ui->lineEdit_ki->hasFocus()){
        QString ki = QString("%1").arg(QtSystem.pid.Ki);
        ui->lineEdit_ki->setText(ki);
    }
    if(!ui->lineEdit_kd->hasFocus()){
        QString kd = QString("%1").arg(QtSystem.pid.Kd);
        ui->lineEdit_kd->setText(kd);
    }
    if(!ui->lineEdit_deltak->hasFocus()){
        QString deltak = QString("%1").arg(QtSystem.deltak);
        ui->lineEdit_deltak->setText(deltak);
    }
}

void DebugPID::on_pushButton_Upload_clicked()
{
    sendCmd(0x0095,0x0001,1);
    pidtimer->start(500);
}

void DebugPID::on_pushButton_flash_clicked()
{
    sendCmd(0x0095,0x0101,1);
}

void DebugPID::on_pushButton_Download_clicked()
{
    double mode = ui->lineEdit_mode->text().toDouble();
    int imode = round(mode);
    sendCmd(UIaddr_pid_mode,imode,1);
    Delay_MSec(200);

    double kp = ui->lineEdit_kp->text().toDouble();
    sendCmd(UIaddr_pid_p,kp,1000);
    Delay_MSec(200);

    double ki = ui->lineEdit_ki->text().toDouble();
    sendCmd(UIaddr_pid_i,ki,1000);
    Delay_MSec(200);

    double kd = ui->lineEdit_kd->text().toDouble();
    sendCmd(UIaddr_pid_d,kd,1000);
    Delay_MSec(200);

    double deltak = ui->lineEdit_deltak->text().toDouble();
    sendCmd(UIaddr_pid_delta_k,deltak,1000);
    Delay_MSec(200);
}
