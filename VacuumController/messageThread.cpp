#include "messageThread.h"
#include "message.h"

MessageThread::MessageThread(QObject *parent)
{

}

void MessageThread::run(void)
{
    qDebug() << "receive the execute signal" ;
    qDebug() << "\tCurrent thread ID: " << QThread::currentThreadId();

    // 循环
    while(1)
    {
        extract_command();
    }
    // 发送结束信号
    qDebug() << "\tFinish the work and sent the result Ready signal\n" ;

    emit resultReady();
}
