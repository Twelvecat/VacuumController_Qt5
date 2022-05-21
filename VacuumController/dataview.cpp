#include "dataview.h"
#include "ui_dataview.h"

DataView::DataView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataView)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setAcceptDrops(false);  //控件或者窗口
    open_ok=0;
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
    pEngViewC->show();
}

DataView::~DataView()
{
    delete ui;
}

void DataView::GetFilePath(QString filepath)
{
    this->setWindowTitle(QString::fromLocal8Bit("数据加载中，请稍等..."));
    filename = filepath;
    open_ok = 1;
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
    while(!in.atEnd())
    {
        QStringList tempData = in.readLine().split(",");
        for(int i = 0;i < tempData.count()-1;i++)
        {
        //读取每一列
        Data[i] = tempData.at(i).toFloat();
        }
        fig(Data);
    }
    file.close();
    this->setWindowTitle(filename);
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
    pEngViewC->page()->runJavaScript(strVal);
}

void DataView::resizeEvent(QResizeEvent *event){
    pEngViewC->resize(this->width(),this->height());
}
