#include "dataview.h"
#include "ui_dataview.h"

DataView::DataView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataView)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setAcceptDrops(false);  //控件或者窗口
    open_ok=0;
    filelen=0;
    //曲线
    pEngViewC = new QWebEngineView(this);
    connect(pEngViewC, &QWebEngineView::loadFinished, this, &DataView::openfile);
    QColor backcolor(40, 44, 52);
    pEngViewC->page()->setBackgroundColor(backcolor);
    pEngViewC->setContextMenuPolicy(Qt::NoContextMenu);
    pEngViewC->resize(this->width(),this->height());
    pEngViewC->move(0,0);
    //pEngViewC->lower();
    pEngViewC->load(QUrl(qApp->applicationDirPath() + "/htmlEcharts/dataview.html"));//这句是调用本地html文件的地址，前面讲过要将testEcharts.html文件和main.echarts.js文件放在同一个文件夹htmlEcharts中。
    pEngViewC->hide();
    ui->water_process->setUsedColor(QColor(79, 175, 243));
    ui->water_process->setValue(0);
    ui->water_process->setBorderWidth(10);

    stop_load = new QAction(QString::fromLocal8Bit("终止加载"),this);
    connect(stop_load,SIGNAL(triggered()),this,SLOT(stop_load_line()));

    // 方式1
//    QPalette palette = QPalette();
//    palette.setColor(QPalette::Background, QColor(0x00,0xFF,0x00,0x00));
//    setPalette(palette);
}

DataView::~DataView()
{
    delete ui;
}

void DataView::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *menu = new QMenu();
    menu->setFixedWidth(100);
    menu->addAction(stop_load);
    menu->exec(e->globalPos());
    delete menu;
}

void DataView::stop_load_line()
{
    pEngViewC->stop();
    pEngViewC->load(QUrl("www.baidu.com"));
    close();
}

void DataView::GetFilePath(QString filepath)
{
    this->setWindowTitle(QString::fromLocal8Bit("数据加载中，请稍等..."));
    filename = filepath;
    open_ok = 1;
}

void DataView::GetViewLength(QString len)
{
    long long int lenint = len.toInt();
    int poccess = round((100*lenint/filelen));
    QString title = QString::number(poccess) + QString::fromLocal8Bit("% 数据加载中，加载期间请勿关闭本窗口，请稍等...");
    this->setWindowTitle(title);
    ui->water_process->setValue(poccess);
    if(lenint == filelen)
    {
        hide();
        this->setWindowFlags(Qt::Window);
        this->setWindowTitle(filename);
        show();
        ui->water_process->hide();
        resize(1000,750);
        pEngViewC->show();
    }
}

void DataView::openfile(void){
    while(!open_ok);
    float Data[7] ={0};
    QFile file;
    if(filename.isNull())
    {
        return;
    }
    file.setFileName(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"open file fail";
        return;
    }
    QTextStream in(&file);             //创建一个文本流
    //读数据
    QStringList headData = in.readLine().split(",");
    if(headData.count()<6){
        file.close();
        hide();
        this->setWindowFlags(Qt::Window);
        show();
        this->setWindowTitle(QString::fromLocal8Bit("文件错误，请导入本软件生成的数据文件！"));
        ui->water_process->setUsedColor(QColor(253, 115, 71));
        resize(500,150);
        return;
    }
    if(headData.at(0) != "Pipe_Pres" || headData.at(6) != "Pump_Output"){
        file.close();
        hide();
        this->setWindowFlags(Qt::Window);
        show();
        this->setWindowTitle(QString::fromLocal8Bit("文件错误，请导入本软件生成的数据文件！"));
        ui->water_process->setUsedColor(QColor(253, 115, 71));
        resize(500,150);
        return;
    }
    while(!in.atEnd())
    {
        QStringList tempData = in.readLine().split(",");
        for(int i = 0;i < 7;i++)
        {
        //读取每一列
        Data[i] = tempData.at(i).toFloat();
        }
        fig(Data);
        filelen++;
    }
    file.close();
}

void DataView::fig(float* data)
{
    QJsonObject seriesData;
    QJsonArray data_Pipe_Pres = {round(data[0]*10)/10.0};
    seriesData.insert("data_Pipe_Pres", data_Pipe_Pres);
    QJsonArray data_Ambient_Pres = {round(data[1]*10)/10.0};
    seriesData.insert("data_Ambient_Pres", data_Ambient_Pres);
    QJsonArray data_Pipe_Temp = {round(data[2]*10)/10.0};
    seriesData.insert("data_Pipe_Temp", data_Pipe_Temp);
    QJsonArray data_Chip_Temp = {round(data[3]*10)/10.0};
    seriesData.insert("data_Chip_Temp", data_Chip_Temp);
    QJsonArray data_Current_nPressure = {round(data[4]*10)/10.0};
    seriesData.insert("data_Current_nPressure", data_Current_nPressure);
    QJsonArray data_Set_nPressure = {round(data[5]*10)/10.0};
    seriesData.insert("data_Set_nPressure", data_Set_nPressure);
    QJsonArray data_Pump_Output = {round(data[6]*10)/10.0};
    seriesData.insert("data_Pump_Output", data_Pump_Output);

    QString str = QString(QJsonDocument(seriesData).toJson());      //将Json格式转为QString
    str = str.remove("\t").remove('\n');                            //移除多余的制表和换行
    QString strVal = QString("updata(%1);").arg(str);        //转为标准字符串
    pEngViewC->page()->runJavaScript(strVal,[this](const QVariant &v) {this->GetViewLength(v.toString());});
}

void DataView::resizeEvent(QResizeEvent *event){
    pEngViewC->resize(this->width(),this->height());
}


void DataView::mousePressEvent(QMouseEvent *e)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = e->globalPos();
}

void DataView::mouseMoveEvent(QMouseEvent *e)
{
    if (isPressedWidget)
    {
        int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx, y()+dy);
    }
}

void DataView::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}
