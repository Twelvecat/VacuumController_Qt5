#ifndef MESSAGETHREAD_H
#define MESSAGETHREAD_H

#include <QObject>
#include <QDebug>
#include <QThread>

class MessageThread : public QObject
{
    Q_OBJECT

   int end;

    public:
        explicit MessageThread(QObject *parent = nullptr);

    public slots:
        void run(void);  // run 定义了线程要执行的操作
        void kill_Thread(void);

    signals:
        void resultReady(void);  // 线程完成工作时发送的信号
};


#endif // MESSAGETHREAD_H
