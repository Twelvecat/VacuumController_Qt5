#include "saveDataThread.h"
#include "mainwindow.h"
#include "VacuumController.h"
#include "ui_mainwindow.h"

saveDataThread::saveDataThread(QObject *parent)
{

}

void saveDataThread::run(void)
{
    qDebug() << "receive the execute signal" ;
    qDebug() << "\tCurrent thread ID: " << QThread::currentThreadId();
    // 循环
    QTime _Timer;
    while(1){
        if(QtSystem.WriteData_Enable)saveData(QtSystem.fileName);
        //延时
        _Timer = QTime::currentTime().addMSecs(1000);
        while( QTime::currentTime() < _Timer ) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    // 发送结束信号
    qDebug() << "\tFinish the work and sent the result Ready signal\n" ;

    emit resultReady();
}
