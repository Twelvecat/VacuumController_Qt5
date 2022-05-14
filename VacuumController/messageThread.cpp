#include "messageThread.h"
#include "message.h"
#include <QApplication>

MessageThread::MessageThread(QObject *parent)
{
    end = 1;
}


void MessageThread::kill_Thread(void)
{
    end = 0;
}

void MessageThread::run(void)
{
    qDebug() << "receive the execute signal" ;
    qDebug() << "\tCurrent thread ID: " << QThread::currentThreadId();
    // 循环
    while(end)
    {
        QApplication::processEvents();
        extract_command(&QtSystem);
    }
    // 发送结束信号
    qDebug() << "\tFinish the work and sent the result Ready signal\n" ;

    emit resultReady();
}
