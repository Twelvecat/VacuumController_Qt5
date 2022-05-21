#ifndef DEBUGPID_H
#define DEBUGPID_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class DebugPID;
}

class DebugPID : public QWidget
{
    Q_OBJECT
    QTcpSocket *pid_socket;
    QTimer *pidtimer;

public:
    explicit DebugPID(QWidget *parent = nullptr);
    ~DebugPID();
    void linkTcp(QTcpSocket *socket);
    void SendBuffer(uint8_t *ptr,int len);
    void sendCmd(uint16_t addr,float value, int K);
    void Delay_MSec(unsigned int msec);

private slots:

    void on_pushButton_Upload_clicked();
    void on_pushButton_Download_clicked();
    void on_pushButton_flash_clicked();
    void showpid();

private:
    Ui::DebugPID *ui;
};

#endif // DEBUGPID_H
