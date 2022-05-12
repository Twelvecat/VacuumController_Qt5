#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>
#include <QTextCodec>
#include <QThread>
#include "messageThread.h"
#include <QtWebEngineWidgets/QWebEngineView>
#include <QButtonGroup>
#include <QTimer>
#include <QMessageBox>
#include <QWebEnginePage>
#include <QWebEngineProfile>


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
    QWebEngineView *pEngView;
    QWebEngineView *pEngViewC;
    QButtonGroup *m_btnGroup1;
    QButtonGroup *m_btnGroup2;
    QTimer *timer;
    QWebEngineProfile *profile;

private slots:

    void on_pushButton_Connect_clicked();

    void on_pushButton_Send_clicked();

    void on_pushButton_Run_clicked();

    void on_pushButton_Stop_clicked();

    void socket_Read_Data();

    void socket_Disconnected();

    static void handleResults(void);  // 处理子线程执行的结果

    void on_bgGroup1_toggled(int);
    void on_bgGroup2_toggled(int);
    void valueChanged_change(int);
    void valueChanged_PWM(int);
    void lineEdit_setPres_change(void);
    void lineEdit_setTime_change(void);
    void updataView(void);
    void on_WebDownload(QWebEngineDownloadItem *item);

signals:
    void opt_command(void);  // 发送信号，触发线程
};
#endif // MAINWINDOW_H
