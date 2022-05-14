#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "message.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

extern uint16_t TalNum;
extern uint8_t CommBuff[BUFFER_SIZE];//定义指令缓冲区

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 颜色调教
    ui->label_ip->setStyleSheet("color: #EAEBEB;");
    ui->label_port->setStyleSheet("color: #EAEBEB;");
    ui->label_relay1->setStyleSheet("color: #EAEBEB;");
    ui->label_relay2->setStyleSheet("color: #EAEBEB;");
    ui->label_setnp->setStyleSheet("color: #EAEBEB;");
    ui->label_curnp->setStyleSheet("color: #EAEBEB;");
    ui->label_current_pres->setStyleSheet("color: #EAEBEB;");
    ui->label_runtime->setStyleSheet("color: #EAEBEB;");
    ui->label_run_time->setStyleSheet("color: #EAEBEB;");
    ui->label_pumpout->setStyleSheet("color: #EAEBEB;");
    ui->label_pwm->setStyleSheet("color: #EAEBEB;");
    ui->label_auto->setStyleSheet("color: #EAEBEB;");
    ui->label_man->setStyleSheet("color: #EAEBEB;");
    ui->label_settime->setStyleSheet("color: #EAEBEB;");
    ui->label_retime->setStyleSheet("color: #EAEBEB;");
    ui->label_rectime->setStyleSheet("color: #EAEBEB;");
    ui->label_pumpout2->setStyleSheet("color: #EAEBEB;");
    ui->label_man_pwm->setStyleSheet("color: #EAEBEB;");
    ui->radioButton_1O->setStyleSheet("color: #EAEBEB;");
    ui->radioButton_1C->setStyleSheet("color: #EAEBEB;");
    ui->radioButton_2O->setStyleSheet("color: #EAEBEB;");
    ui->radioButton_2C->setStyleSheet("color: #EAEBEB;");
    ui->groupBox->setStyleSheet("QGroupBox {border:1px solid #EAEBEB;color: #EAEBEB;margin-top:6px;border-radius: 10px;}QGroupBox::title { subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    ui->groupBox_2->setStyleSheet("QGroupBox {border:1px solid #EAEBEB;color: #EAEBEB;margin-top:6px;border-radius: 10px;}QGroupBox::title { subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    ui->groupBox_5->setStyleSheet("QGroupBox {border:1px solid #EAEBEB;color: #EAEBEB;margin-top:6px;border-radius: 10px;}QGroupBox::title { subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    ui->groupBox_6->setStyleSheet("QGroupBox {border:1px solid #EAEBEB;color: #EAEBEB;margin-top:6px;border-radius: 10px;}QGroupBox::title { subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    ui->groupBox_3->setStyleSheet("QGroupBox {border:1px solid #EAEBEB;color: #EAEBEB;margin-top:6px;border-radius: 10px;}QGroupBox::title { subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    ui->groupBox_4->setStyleSheet("QGroupBox {border:1px solid #EAEBEB;color: #EAEBEB;margin-top:6px;border-radius: 10px;}QGroupBox::title { subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    ui->pushButton_Connect->setStyleSheet("\
        QPushButton{color:#EAEBEB;background-color:#282c34;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
        QPushButton::hover{color:#282c34;background-color:#EAEBEB;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
        QPushButton::pressed{color:#EAEBEB;background-color:#464C59;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
    ");
    ui->pushButton_Run->setStyleSheet("\
        QPushButton{color:#EAEBEB;background-color:#282c34;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
        QPushButton::hover{color:#282c34;background-color:#EAEBEB;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
        QPushButton::pressed{color:#EAEBEB;background-color:#464C59;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
    ");
    ui->pushButton_Stop->setStyleSheet("\
        QPushButton{color:#EAEBEB;background-color:#282c34;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
        QPushButton::hover{color:#282c34;background-color:#EAEBEB;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
        QPushButton::pressed{color:#EAEBEB;background-color:#464C59;border-style:outset;border-radius: 10px;border-width:1px;border-color:#EAEBEB;}\
    ");
    ui->lineEdit_IP->setStyleSheet("\
        QLineEdit{border-radius: 5px;background: #464C59;color:#EAEBEB;}\
    ");
    ui->lineEdit_Port->setStyleSheet("\
        QLineEdit{border-radius: 5px;background: #464C59;color:#EAEBEB;}\
    ");
    ui->lineEdit_setPres->setStyleSheet("\
        QLineEdit{border-radius: 5px;background: #464C59;color:#EAEBEB;}\
    ");
    ui->lineEdit_setTime->setStyleSheet("\
        QLineEdit{border-radius: 5px;background: #464C59;color:#EAEBEB;}\
    ");
    this->setWindowTitle(QString::fromLocal8Bit("真空负压控制器"));
    this->setWindowIcon(QIcon(qApp->applicationDirPath() +"/htmlEcharts/VC.ico"));
    //this->setAttribute(Qt::WA_DeleteOnClose,true);


    /*通讯部份*/
    socket = new QTcpSocket(this);
    //连接信号槽
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    //ui->pushButton_Send->setEnabled(false);
    ui->lineEdit_IP->setText("192.168.1.1");
    ui->lineEdit_Port->setText("8080");
    QtSystem.linkTcp(socket);

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
    // 终止循环信号
    connect(this, SIGNAL(kill_mesgThread(void)), messageThread, SLOT(kill_Thread(void)));
    // 启动线程
    mesgThread.start();
    // 发射信号，开始执行
    qDebug() << "emit the signal to execute!" ;
    qDebug() << "\tCurrent thread ID:" << QThread::currentThreadId() << '\n' ;
    emit opt_command();


    /*保存文件线程*/
    auto *savedataThread = new saveDataThread;
    savedataThread->moveToThread(&saveThread);
    connect(this, SIGNAL(opt_save(void)), savedataThread, SLOT(run(void)));
    connect(&saveThread, &QThread::finished, savedataThread, &QObject::deleteLater);
    connect(savedataThread, SIGNAL(resultReady(void)), this, SLOT(handlesaveResults(void)));
    connect(this, SIGNAL(kill_saveThread(void)), savedataThread, SLOT(kill_Thread(void)));
    saveThread.start();
    // 发射信号，开始执行
    qDebug() << "emit the signal to execute!" ;
    qDebug() << "\tCurrent thread ID:" << QThread::currentThreadId() << '\n' ;
    emit opt_save();

    //仪表盘
    pEngView = new QWebEngineView(this);
    pEngView->setBackgroundRole(QPalette::Window);
    pEngView->setContextMenuPolicy(Qt::NoContextMenu);
    pEngView->resize(561,321);
    pEngView->move(25,30);
    pEngView->lower();
    pEngView->load(QUrl(qApp->applicationDirPath() + "/htmlEcharts/panel.html"));//这句是调用本地html文件的地址，前面讲过要将testEcharts.html文件和main.echarts.js文件放在同一个文件夹htmlEcharts中。
    pEngView->show();
    //曲线
    pEngViewC = new QWebEngineView(this);
    pEngViewC->setBackgroundRole(QPalette::Window);
    pEngViewC->setContextMenuPolicy(Qt::NoContextMenu);
    pEngViewC->resize(800,600);
    pEngViewC->move(600,30);
    //pEngViewC->lower();
    pEngViewC->load(QUrl(qApp->applicationDirPath() + "/htmlEcharts/curve.html"));//这句是调用本地html文件的地址，前面讲过要将testEcharts.html文件和main.echarts.js文件放在同一个文件夹htmlEcharts中。
    pEngViewC->show();
    profile = pEngViewC->page()->profile();
    connect(profile,SIGNAL(downloadRequested(QWebEngineDownloadItem *)),this,SLOT(on_WebDownload(QWebEngineDownloadItem *)));

    // 继电器1
    m_btnGroup1 = new QButtonGroup(this);
    m_btnGroup1->addButton(ui->radioButton_1O,1);
    m_btnGroup1->addButton(ui->radioButton_1C,0);
    ui->radioButton_1C->setChecked(1);
    connect(m_btnGroup1, SIGNAL(buttonClicked(int)), this, SLOT(on_bgGroup1_toggled(int)));
    // 继电器2
    m_btnGroup2 = new QButtonGroup(this);
    m_btnGroup2->addButton(ui->radioButton_2O,1);
    m_btnGroup2->addButton(ui->radioButton_2C,0);
    ui->radioButton_2C->setChecked(1);
    connect(m_btnGroup2, SIGNAL(buttonClicked(int)), this, SLOT(on_bgGroup2_toggled(int)));


    // 设置是否启动滑块追踪：true 表示启动追踪；false 表示禁用追踪；
    // 如果启用了滑块追踪，则滑块在拖动过程中会一直触发 valueChanged 信号；
    // 如果禁用了滑块追踪，则滑块在拖动过程中不会触发 valueChanged 信号，
    // 只有在用户释放滑块时，才触发 valueChanged 信号
    ui->horizontalSlider_PWM->setTracking(true);
    connect(ui->horizontalSlider_PWM,SIGNAL(valueChanged(int)),this,SLOT(valueChanged_PWM(int)));
    connect(ui->horizontalSlider_PWM,SIGNAL(sliderReleased(void)),this,SLOT(sliderReleased_PWM(void)));

    //手自页面
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->verticalSlider_switch,SIGNAL(valueChanged(int)),this,SLOT(valueChanged_change(int)));

    connect(ui->lineEdit_setPres,SIGNAL(returnPressed(void)),this,SLOT(lineEdit_setPres_change(void)));
    connect(ui->lineEdit_setTime,SIGNAL(returnPressed(void)),this,SLOT(lineEdit_setTime_change(void)));

    ui->pushButton_Stop->setEnabled(false);

    timer = new QTimer(this);
    connect(timer, SIGNAL (timeout(void)), this, SLOT(updataView(void)));
    timer->start(200);// 单位毫秒

    timer_curve = new QTimer(this);
    connect(timer_curve, SIGNAL (timeout(void)), this, SLOT(updataCurve(void)));
    timer_curve->start(1000);// 单位毫秒

    m_btnGroup1_isbusy = 0;
    m_btnGroup2_isbusy = 0;
    button_run_isbusy = 0;
    button_stop_isbusy = 0;
    Slider_PWM_isbusy = 0;
}


MainWindow::~MainWindow()
{
    emit kill_mesgThread();
    emit kill_saveThread();
    // 析构函数中调用 quit() 函数结束线程
    mesgThread.quit();
    mesgThread.wait();
    saveThread.quit();
    saveThread.wait();
//    delete this->socket;
//    delete pEngView;
//    delete pEngViewC;
//    delete m_btnGroup1;
//    delete m_btnGroup2;
//    delete timer;
//    delete timer_curve;
    delete ui;
}

void MainWindow::on_WebDownload(QWebEngineDownloadItem *item)
{
    QDateTime timeCurrent = QDateTime::currentDateTime();
    QString time = timeCurrent.toString("[yy-MM-dd_hh_mm_ss]");
    QString path = qApp->applicationDirPath()+"/svg_download/"+ time +".svg";
    if (path.isEmpty())
        return;
    qDebug() << path;
    item->setPath(path);
    item->accept();
}

bool MainWindow::DirExistEx(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
        return true;
    }
    else
    {
        //不存在当前目录，创建，可创建多级目录
        bool ok = dir.mkpath(fullPath);
        return ok;
    }
}

QString MainWindow::CreateFile(){
    int col = 7;
    QString tableName[7] = {"Pipe_Pres","Ambient_Pres","Pipe_Temp","Chip_Temp","Current_nPressure","Set_nPressure","Pump_Output"};
    QFile file;
    QDateTime timeCurrent = QDateTime::currentDateTime();
    QString time = timeCurrent.toString("[yy-MM-dd_hh_mm_ss]");
    QString DirName = qApp->applicationDirPath()+"/Data_download/";
    QString fileName = DirName + time +".csv";

    if(!DirExistEx(DirName)){
        qDebug() << "mkdir fail";
        return "";
    }
    if(fileName.isNull())
    {
        return "";
    }
    file.setFileName(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"open file fail";
        return "";
    }
    QTextStream out(&file);             //创建一个文本流，向保存文件中写入文本
    //写表头
    for(int i = 0;i < col; i++)
    {
        out<<tableName[i]<<",";
        if(i == (col - 1))
        {
            out<<"\n";
        }
    }
    file.close();
    qDebug() << fileName;
    return fileName;
}

void saveData(QString fileName){
    //以下是保存csv
    float Data[7] ={0};
    int col = 7;
    QFile file;
    if(fileName.isNull())
    {
        return;
    }
    file.setFileName(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        qDebug()<<"open file fail";
        return;
    }
    QtSystem.File_isClose = 0;
    QTextStream out(&file);             //创建一个文本流，向保存文件中写入文本
    //写数据
    int i = 1;
    while(i--)
    {
        qDebug() << "writing";
        Data[0] = round(QtSystem.hp5806_B.Pcomp*10)/10.0;
        Data[1] = round(QtSystem.hp5806_A.Pcomp*10)/10.0;
        Data[2] = round(QtSystem.hp5806_B.Tcomp*10)/10.0;
        Data[3] = round(QtSystem.hp5806_A.Tcomp*10)/10.0;
        Data[4] = round(QtSystem.current_pres*10)/10.0;
        Data[5] = round(QtSystem.set_value*10)/10.0;
        Data[6] = round(QtSystem.output_value*10)/10.0;
        for(int j = 0;j < col; j++)
        {
            out<<Data[j]<<",";
            if(j == (col - 1))
            {
                out<<"\n";
            }
        }
    }
    file.close();
    QtSystem.File_isClose = 1;
}

void MainWindow::updataCurve()
{
    QJsonObject seriesData;
    QJsonArray data_Pipe_Pres = {round(QtSystem.hp5806_B.Pcomp*10)/10.0};
    seriesData.insert("data_Pipe_Pres", data_Pipe_Pres);
    QJsonArray data_Ambient_Pres = {round(QtSystem.hp5806_A.Pcomp*10)/10.0};
    seriesData.insert("data_Ambient_Pres", data_Ambient_Pres);
    QJsonArray data_Pipe_Temp = {round(QtSystem.hp5806_B.Tcomp*10)/10.0};
    seriesData.insert("data_Pipe_Temp", data_Pipe_Temp);
    QJsonArray data_Chip_Temp = {round(QtSystem.hp5806_A.Tcomp*10)/10.0};
    seriesData.insert("data_Chip_Temp", data_Chip_Temp);
    QJsonArray data_Current_nPressure = {round(QtSystem.current_pres*10)/10.0};
    seriesData.insert("data_Current_nPressure", data_Current_nPressure);
    QJsonArray data_Set_nPressure = {round(QtSystem.set_value*10)/10.0};
    seriesData.insert("data_Set_nPressure", data_Set_nPressure);

    QString str = QString(QJsonDocument(seriesData).toJson());      //将Json格式转为QString
    str = str.remove("\t").remove('\n');                            //移除多余的制表和换行
    QString strVal = QString("updata(%1);").arg(str);        //转为标准字符串
    pEngViewC->page()->runJavaScript(strVal);
}


void MainWindow::updataView()
{
    // 仪表盘
    QJsonObject seriesData;
    QJsonArray data_pres = {round(QtSystem.hp5806_B.Pcomp*10)/10.0};
    seriesData.insert("data_pres", data_pres);
    QJsonArray data_temp = {round(QtSystem.hp5806_B.Tcomp*10)/10.0};
    seriesData.insert("data_temp", data_temp);
    QString str = QString(QJsonDocument(seriesData).toJson());      //将Json格式转为QString
    str = str.remove("\t").remove('\n');                            //移除多余的制表和换行
    QString strVal = QString("updata(%1);").arg(str);        //转为标准字符串
    pEngView->page()->runJavaScript(strVal);
    // 继电器
    if(!m_btnGroup1_isbusy)
        m_btnGroup1->button(QtSystem.relay_A.status)->setChecked(true);
    if(!m_btnGroup2_isbusy)
        m_btnGroup2->button(QtSystem.relay_B.status)->setChecked(true);
    // 状态切换
    if(!button_run_isbusy && !button_stop_isbusy){
        if(QtSystem.system_status.current == 1){
            QtSystem.WriteData_Enable = 0;
            if(ui->pushButton_Run->text() != QString::fromLocal8Bit("启动"))
            {
                on_pushButton_Stop_clicked();
            }
        }
        else if(QtSystem.system_status.current == 2){
            QtSystem.WriteData_Enable = 1;
            if(ui->pushButton_Run->text() != QString::fromLocal8Bit("暂停"))
            {
                if(ui->pushButton_Run->text() != QString::fromLocal8Bit("继续"))
                {
                    while(!QtSystem.File_isClose);
                    QtSystem.fileName = CreateFile();
                }
                ui->pushButton_Stop->setEnabled(true);
                ui->pushButton_Run->setText(QString::fromLocal8Bit("暂停"));
            }
            if(ui->stackedWidget->currentIndex() != 0)ui->stackedWidget->setCurrentIndex(0);
            if(ui->verticalSlider_switch->value() != 1)ui->verticalSlider_switch->setValue(1);
        }
        else if(QtSystem.system_status.current == 3){
            QtSystem.WriteData_Enable = 1;
            if(ui->pushButton_Run->text() != QString::fromLocal8Bit("继续"))
            {
                ui->pushButton_Stop->setEnabled(true);
                ui->pushButton_Run->setText(QString::fromLocal8Bit("继续"));
            }
            if(ui->stackedWidget->currentIndex() != 0)ui->stackedWidget->setCurrentIndex(0);
            if(ui->verticalSlider_switch->value() != 1)ui->verticalSlider_switch->setValue(1);
        }
        else if(QtSystem.system_status.current == 4){
            QtSystem.WriteData_Enable = 1;
            if(ui->pushButton_Run->text() != QString::fromLocal8Bit("暂停"))
            {
                if(ui->pushButton_Run->text() != QString::fromLocal8Bit("继续"))
                {
                    while(!QtSystem.File_isClose);
                    QtSystem.fileName = CreateFile();
                }
                ui->pushButton_Stop->setEnabled(true);
                ui->pushButton_Run->setText(QString::fromLocal8Bit("暂停"));
            }
            if(ui->stackedWidget->currentIndex() != 1)ui->stackedWidget->setCurrentIndex(1);
            if(ui->verticalSlider_switch->value() != 0)ui->verticalSlider_switch->setValue(0);
        }
        else if(QtSystem.system_status.current == 5){
            QtSystem.WriteData_Enable = 1;
            if(ui->pushButton_Run->text() != QString::fromLocal8Bit("继续"))
            {
                ui->pushButton_Stop->setEnabled(true);
                ui->pushButton_Run->setText(QString::fromLocal8Bit("继续"));
            }
            if(ui->stackedWidget->currentIndex() != 1)ui->stackedWidget->setCurrentIndex(1);
            if(ui->verticalSlider_switch->value() != 0)ui->verticalSlider_switch->setValue(0);

        }
        else if(QtSystem.system_status.current == 6){
            QtSystem.WriteData_Enable = 0;
            QMessageBox::critical(this, QString::fromLocal8Bit("触发急停警报"),
                                  QString::fromLocal8Bit("设备机械急停按钮被触发，已关闭真空泵，终止所有任务！\n请在确保设备安全的情况下手动长按 屏幕右侧机械按键解除警报。"),
                                  QString::fromLocal8Bit("已解除"));
        }
        else if(QtSystem.system_status.current == 7){
            QtSystem.WriteData_Enable = 0;
        }
    }
    // 自动页面
    if(!ui->lineEdit_setPres->hasFocus()){
        QString str_set_value = QString("%1").arg(QtSystem.set_value,0,'f',1);
        ui->lineEdit_setPres->setText(str_set_value);
    }
    QString str_current_pres = QString("%1 hPa").arg(QtSystem.hp5806_A.Pcomp - QtSystem.hp5806_B.Pcomp,0,'f',1);
    ui->label_current_pres->setText(str_current_pres);
    QString str_run_time = QString::fromLocal8Bit("%1时%2分%3秒").arg(QtSystem.time.hour).arg(QtSystem.time.min).arg(QtSystem.time.sec);
    ui->label_run_time->setText(str_run_time);
    QString str_pwm = QString("%1\%").arg(QtSystem.output_value);
    ui->label_pwm->setText(str_pwm);
    // 手动页面
    if(!ui->lineEdit_setTime->hasFocus()){
        QString str_set_Time = QString("%1").arg(QtSystem.time.setting_sec);
        ui->lineEdit_setTime->setText(str_set_Time);
    }
    if(QtSystem.time.setting_sec == -1)
    {
        QString str_rectime = QString::fromLocal8Bit("无限");
        ui->label_rectime->setText(str_rectime);
    }
    else{
        QString str_rectime = QString::fromLocal8Bit("%1时%2分%3秒").arg(QtSystem.time.remainder_sec/3600).arg(QtSystem.time.remainder_sec%3600/60).arg(QtSystem.time.remainder_sec%3600%60);
        ui->label_rectime->setText(str_rectime);
    }
    if(!Slider_PWM_isbusy && !ui->horizontalSlider_PWM->hasFocus()){
            QString str_label_man_pwm = QString("%1\%").arg(QtSystem.output_manual);
            ui->label_man_pwm->setText(str_label_man_pwm);
            ui->horizontalSlider_PWM->setValue(QtSystem.output_manual);
    }


   // 安全事件
    if(QtSystem.safeEvent_status == 0x0001){
        QMessageBox::warning(this, QString::fromLocal8Bit("触发温度警报"),
                              QString::fromLocal8Bit("传感器检测到温度过高！\n若持续升高可能会损坏传感器，或造成数据不准确，请尽快将设备转移到合适工作温度处或暂时关闭设备。"),
                              QString::fromLocal8Bit("已知晓"));
        if(QtSystem.safeEvent_status==0x0001)//因为下位机可能改变警告，受优先队列影响，上位机需要忽略本次，重新更新
        QtSystem.Know();
    }
    else if(QtSystem.safeEvent_status == 0x0002){
        QMessageBox::warning(this, QString::fromLocal8Bit("触发温度警报"),
                              QString::fromLocal8Bit("传感器检测到温度过低！\n若持续降低可能会损坏传感器，或造成数据不准确，请尽快将设备转移到合适工作温度处或暂时关闭设备。"),
                              QString::fromLocal8Bit("已知晓"));
        if(QtSystem.safeEvent_status==0x0002)
        QtSystem.Know();
    }
    else if(QtSystem.safeEvent_status == 0x0003){
        QMessageBox::critical(this, QString::fromLocal8Bit("触发气压警报"),
                              QString::fromLocal8Bit("传感器检测到气压过高！\n为防止损坏传感器，已暂停当前任务。请在确保设备安全的情况下恢复或重启任务。"),
                              QString::fromLocal8Bit("已知晓"));
        if(QtSystem.safeEvent_status==0x0003)
        QtSystem.Know();
    }
    else if(QtSystem.safeEvent_status == 0x0004){
        QMessageBox::critical(this, QString::fromLocal8Bit("触发气压警报"),
                              QString::fromLocal8Bit("传感器检测到气压过低！\n为防止损坏传感器，已暂停当前任务。请在确保设备安全的情况下恢复或重启任务。"),
                              QString::fromLocal8Bit("已知晓"));
        if(QtSystem.safeEvent_status==0x0004)
        QtSystem.Know();
    }
    else if(QtSystem.safeEvent_status == 0x0005){
        QMessageBox::warning(this, QString::fromLocal8Bit("触发泄漏警报"),
                              QString::fromLocal8Bit("设备检测到当前抽气管道可能存在泄漏，或接头处松动！\n无法达到设定负压值，请检查管道及各部件连接处。"),
                              QString::fromLocal8Bit("已知晓"));
        if(QtSystem.safeEvent_status==0x0005)
        QtSystem.Know();
    }
}

void MainWindow::on_pushButton_Run_clicked()
{
    button_run_isbusy = 1;
    QtSystem.Run(ui->verticalSlider_switch->value());
    if(ui->pushButton_Run->text() == QString::fromLocal8Bit("启动"))
    {
        ui->pushButton_Stop->setEnabled(true);
        ui->pushButton_Run->setText(QString::fromLocal8Bit("暂停"));
        while(!QtSystem.File_isClose);
        QtSystem.fileName = CreateFile();
    }
    else if(ui->pushButton_Run->text() == QString::fromLocal8Bit("暂停"))
    {
        ui->pushButton_Stop->setEnabled(true);
        ui->pushButton_Run->setText(QString::fromLocal8Bit("继续"));
    }
    else if(ui->pushButton_Run->text() == QString::fromLocal8Bit("继续"))
    {
        ui->pushButton_Stop->setEnabled(true);
        ui->pushButton_Run->setText(QString::fromLocal8Bit("暂停"));
    }
    Delay_MSec(1000);
    button_run_isbusy = 0;
}


void MainWindow::on_pushButton_Stop_clicked()
{
    button_stop_isbusy = 1;
    QtSystem.Stop();
    ui->pushButton_Run->setText(QString::fromLocal8Bit("启动"));
    ui->pushButton_Stop->setEnabled(false);
    Delay_MSec(1000);
    button_stop_isbusy = 0;
}

void MainWindow::Delay_MSec(unsigned int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));  //创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();  //事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void MainWindow::lineEdit_setTime_change(void)
{
    double fTime = ui->lineEdit_setTime->text().toDouble();
    int iTime = round(fTime);
    QtSystem.SetTimeSec(iTime);
    Delay_MSec(1000);
    ui->lineEdit_setTime->clearFocus();
}

void MainWindow::lineEdit_setPres_change(void)
{
    double Pres = ui->lineEdit_setPres->text().toDouble();
    QtSystem.SetValue(Pres);
    Delay_MSec(1000);
    ui->lineEdit_setPres->clearFocus();
}

void MainWindow::valueChanged_PWM(int value)
{
    QString str_label_man_pwm = QString("%1\%").arg(value);
    ui->label_man_pwm->setText(str_label_man_pwm);
}


void MainWindow::sliderReleased_PWM()
{
    Slider_PWM_isbusy = 1;
    int value = ui->horizontalSlider_PWM->value();
    QtSystem.SetPWM(value);
    qDebug() << value;
    ui->horizontalSlider_PWM->clearFocus();
    Delay_MSec(1000);
    Slider_PWM_isbusy = 0;
}

void MainWindow::valueChanged_change(int value)
{
    if(value==1)
        ui->stackedWidget->setCurrentIndex(0);
    else
        ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_bgGroup1_toggled(int id)
{
    m_btnGroup1_isbusy = 1;
    QtSystem.OpenOrCloseRelay(m_btnGroup1->checkedId(),1);
    Delay_MSec(1000);
    m_btnGroup1_isbusy = 0;

}

void MainWindow::on_bgGroup2_toggled(int id)
{
    m_btnGroup2_isbusy = 1;
    QtSystem.OpenOrCloseRelay(m_btnGroup2->checkedId(),2);
    Delay_MSec(1000);
    m_btnGroup2_isbusy = 0;
}

void MainWindow::handleResults(void)
{
    qDebug() << "receive the resultReady signal" ;
    qDebug() << "\tCurrent thread ID: " << QThread::currentThreadId() << '\n' ;
}

void MainWindow::handlesaveResults(void)
{
    qDebug() << "receive the resultReady signal" ;
    qDebug() << "\tCurrent thread ID: " << QThread::currentThreadId() << '\n' ;
}

void MainWindow::on_pushButton_Connect_clicked()
{
    if(ui->pushButton_Connect->text() == QString::fromLocal8Bit("连接"))
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
        ///ui->pushButton_Send->setEnabled(true);
        //修改按键文字
        ui->pushButton_Connect->setText(QString::fromLocal8Bit("断开"));
    }
    else
    {
        //断开连接
        socket->disconnectFromHost();
        //修改按键文字
        ui->pushButton_Connect->setText(QString::fromLocal8Bit("连接"));
        //ui->pushButton_Send->setEnabled(false);
        QtSystem.WriteData_Enable = 0;
        QtSystem.system_status.current = 1;
    }
}


void MainWindow::on_pushButton_Send_clicked()
{
    //qDebug() << "Send: " << ui->textEdit_Send->toPlainText();
     //获取文本框内容并以ASCII码形式发送
//    socket->write(ui->textEdit_Send->toPlainText().toLocal8Bit());
//    socket->flush();

//    QByteArray buffer = {0x08,0x00};
//    buffer[0] = 0xA5;
//    buffer[1] = 0x5A;
//    buffer[2] = 0x05;
//    buffer[3] = 0x83;
//    buffer[4] = 0x00;
//    buffer[5] = 0x01;
//    buffer[6] = 0x00;
//    buffer[7] = 0x64;
//    socket->write(buffer);
//    socket->flush();
    QJsonObject seriesData;
    QJsonArray data = {600};
    seriesData.insert("data", data);

    QString str = QString(QJsonDocument(seriesData).toJson());      //将Json格式转为QString
    str = str.remove("\t").remove('\n');                            //移除多余的制表和换行
//    str.replace(QRegExp("\""), "\\\"");	                            //因为转为字符串后需要两次转译
    QString strVal = QString("updata2(%1);").arg(str);        //转为标准字符串
    QString strVall = QString("updata1()");        //转为标准字符串
    pEngView->page()->runJavaScript(strVal);
    qDebug() << str;
    qDebug() << strVal;

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
    //ui->pushButton_Send->setEnabled(false);
    //修改按键文字
    ui->pushButton_Connect->setText(QString::fromLocal8Bit("连接"));
    qDebug() << "Disconnected!";
    QtSystem.WriteData_Enable = 0;
    QtSystem.system_status.current = 1;
}
