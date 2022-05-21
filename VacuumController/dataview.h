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

namespace Ui {
class DataView;
}

class DataView : public QWidget
{
    Q_OBJECT
    QString filename;
    QWebEngineView *pEngViewC;
    int open_ok;

public:
    explicit DataView(QWidget *parent = nullptr);
    ~DataView();
    void GetFilePath(QString filepath);
    void fig(float* data);


private slots:
    void resizeEvent(QResizeEvent *event);
    void openfile(void);

private:
    Ui::DataView *ui;
};

#endif // DATAVIEW_H
