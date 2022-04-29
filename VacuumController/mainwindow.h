#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>
#include <QTextCodec>
#include <QThread>
#include "messageThread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QThread mesgThread;

private slots:

    void on_pushButton_Connect_clicked();

    void on_pushButton_Send_clicked();

    void socket_Read_Data();

    void socket_Disconnected();

    static void handleResults(void);  // 处理子线程执行的结果

signals:
    void opt_command(void);  // 发送信号，触发线程
};
#endif // MAINWINDOW_H
