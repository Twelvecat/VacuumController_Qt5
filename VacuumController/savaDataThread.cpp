#include "saveDataThread.h"
#include "mainwindow.h"
#include "VacuumController.h"
#include "ui_mainwindow.h"
#include <QApplication>

saveDataThread::saveDataThread(QObject *parent)
{
    end = 1;
}

void saveDataThread::kill_Thread(void)
{
    end = 0;
}

void saveDataThread::run(void)
{
    qDebug() << "receive the execute signal" ;
    qDebug() << "\tCurrent thread ID: " << QThread::currentThreadId();
    // 循环
    QTime _Timer;
    while(end){
        QApplication::processEvents();
        if(QtSystem.WriteData_Enable)saveData(QtSystem.fileName);
        //延时
        _Timer = QTime::currentTime().addMSecs(1000);
        while( QTime::currentTime() < _Timer ) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    // 发送结束信号
    qDebug() << "\tFinish the work and sent the result Ready signal\n" ;

    emit resultReady();
}
