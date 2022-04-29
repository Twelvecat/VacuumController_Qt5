#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "message.h"

extern uint16_t TalNum;
extern uint8_t CommBuff[BUFFER_SIZE];//定义指令缓冲区

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*通讯部份*/
    socket = new QTcpSocket();
    //连接信号槽
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    ui->pushButton_Send->setEnabled(false);
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_Port->setText("8080");


    /*分析指令线程部分*/
    auto *messageThread = new MessageThread;
    // 调用 moveToThread 将该任务交给 mesgThread
    messageThread->moveToThread(&mesgThread);
    // operate 信号发射后启动线程工作
    connect(this, SIGNAL(opt_command(void)), messageThread, SLOT(run(void)));
    // 该线程结束时销毁
    connect(&mesgThread, &QThread::finished, messageThread, &QObject::deleteLater);
    // 线程结束后发送信号，对结果进行处理
    connect(messageThread, SIGNAL(resultReady(void)), this, SLOT(handleResults(void)));
    // 启动线程
    mesgThread.start();
    // 发射信号，开始执行
    qDebug() << "emit the signal to execute!" ;
    qDebug() << "\tCurrent thread ID:" << QThread::currentThreadId() << '\n' ;
    emit opt_command();

}

MainWindow::~MainWindow()
{
    // 析构函数中调用 quit() 函数结束线程
    mesgThread.quit();
    mesgThread.wait();
    delete this->socket;
    delete ui;
}

void MainWindow::handleResults(void)
{
    qDebug() << "receive the resultReady signal" ;
    qDebug() << "\tCurrent thread ID: " << QThread::currentThreadId() << '\n' ;
}

void MainWindow::on_pushButton_Connect_clicked()
{
    if(ui->pushButton_Connect->text() == tr("连接"))
    {
        QString IP;
        int port;

        //获取IP地址
        IP = ui->lineEdit_IP->text();
        //获取端口号
        port = ui->lineEdit_Port->text().toInt();

        //取消已有的连接
        socket->abort();
        //连接服务器
        socket->connectToHost(IP, port);

        //等待连接成功
        if(!socket->waitForConnected(30000))
        {
            qDebug() << "Connection failed!";
            return;
        }
        qDebug() << "Connect successfully!";

        //发送按键使能
        ui->pushButton_Send->setEnabled(true);
        //修改按键文字
        ui->pushButton_Connect->setText("断开连接");
    }
    else
    {
        //断开连接
        socket->disconnectFromHost();
        //修改按键文字
        ui->pushButton_Connect->setText("连接");
        ui->pushButton_Send->setEnabled(false);
    }
}


void MainWindow::on_pushButton_Send_clicked()
{
    qDebug() << "Send: " << ui->textEdit_Send->toPlainText();
     //获取文本框内容并以ASCII码形式发送
//    socket->write(ui->textEdit_Send->toPlainText().toLocal8Bit());
//    socket->flush();

    QByteArray buffer = {0x08,0x00};
    buffer[0] = 0xA5;
    buffer[1] = 0x5A;
    buffer[2] = 0x05;
    buffer[3] = 0x83;
    buffer[4] = 0x00;
    buffer[5] = 0x01;
    buffer[6] = 0x00;
    buffer[7] = 0x64;
    socket->write(buffer);
    socket->flush();
}

void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        int size = buffer.size();
        for(int i=0;i<size;i++){
            CommBuff[TalNum++]=buffer[i];//保存串口数据
            if(TalNum==BUFFER_SIZE) TalNum=0;
        }
        //ASCII显示
//        QTextCodec *tc = QTextCodec::codecForName("GBK");
//        QString buffer_str = tc->toUnicode(buffer);//str如果是中文则是中文字符

        //刷新显示
//        QString str = ui->textEdit_Recv->toPlainText();
//        str+=hexData;
//        ui->textEdit_Recv->setText(str);
    }
}

void MainWindow::socket_Disconnected()
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
    //修改按键文字
    ui->pushButton_Connect->setText("连接");
    qDebug() << "Disconnected!";
}
