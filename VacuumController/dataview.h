#ifndef DATAVIEW_H
#define DATAVIEW_H

#include <QWidget>
#include <QDebug>
#include <QString>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QMenu>
#include <QContextMenuEvent>

namespace Ui {
class DataView;
}

class DataView : public QWidget
{
    Q_OBJECT
    QString filename;
    QWebEngineView *pEngViewC;
    int open_ok;
    long long int filelen;
    QPoint last;//窗口拖动用变量
    bool isPressedWidget;
    QAction *stop_load;

public:
    explicit DataView(QWidget *parent = nullptr);
    ~DataView();
    void GetFilePath(QString filepath);
    void fig(float* data);
    void GetViewLength(QString len);


private slots:
    void resizeEvent(QResizeEvent *event);
    void openfile(void);

    //由于去掉显示界面的菜单栏和标题栏，无法拖动窗口，重写关于窗口拖动用的几个函数
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);
    void stop_load_line();

private:
    Ui::DataView *ui;
};

#endif // DATAVIEW_H
