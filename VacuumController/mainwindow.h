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
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include "saveDataThread.h"
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include "dataview.h"
#include "debugpid.h"
#include <QNetworkProxy>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

void saveData(QString fileName);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Delay_MSec(unsigned int msec);
    bool DirExistEx(QString fullPath);
    QString CreateFile();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QThread mesgThread;
    QThread saveThread;
    QWebEngineView *pEngView;
    QWebEngineView *pEngViewC;
    QButtonGroup *m_btnGroup1;
    QButtonGroup *m_btnGroup2;
    QTimer *timer;
    QTimer *timer_curve;
    QWebEngineProfile *profile;
    int m_btnGroup1_isbusy;
    int m_btnGroup2_isbusy;
    int button_run_isbusy;
    int button_stop_isbusy;
    int Slider_PWM_isbusy;


private slots:

    void on_pushButton_Connect_clicked();
    void pushButton_Send_clicked();
    void on_pushButton_Run_clicked();
    void on_pushButton_Stop_clicked();
    void on_pushButton_PID_clicked();
    void socket_Read_Data();
    void socket_Disconnected();
    static void handleResults(void);  // 处理子线程执行的结果
    static void handlesaveResults(void);  // 处理子线程执行的结果
    void bgGroup1_toggled(int);
    void bgGroup2_toggled(int);
    void valueChanged_change(int);
    void valueChanged_PWM(int);
    void lineEdit_setPres_change(void);
    void lineEdit_setTime_change(void);
    void updataView(void);
    void updataCurve(void);
    void on_WebDownload(QWebEngineDownloadItem *item);
    void sliderReleased_PWM();
    void pEngViewCloadFinish();
    virtual void dragEnterEvent(QDragEnterEvent* e) override;
    virtual void dropEvent(QDropEvent* e) override;
    virtual void dragMoveEvent(QDragMoveEvent* e) override;

signals:
    void opt_command(void);  // 发送信号，触发线程
    void opt_save(void);  // 发送信号，触发线程
    void kill_mesgThread(void);
    void kill_saveThread(void);
};
#endif // MAINWINDOW_H
