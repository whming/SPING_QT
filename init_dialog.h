#ifndef INIT_DIALOG_H
#define INIT_DIALOG_H

#include <QWidget>
#include <QTime>
#include <QMessageBox>
#include "sql_api.h"
#include "cJSON.h"
#include "http_manager.h"
#include "main_widget.h"

//获取设备信息
#define DEVICE_ID "/api/pad/deviceId"
//获取下发信息 参数
#define DEVICE_ID_unitPhone "unitPhone="

namespace Ui {
class Init_Dialog;
}

class Init_Dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Init_Dialog(QWidget *parent = 0);
    ~Init_Dialog();
private slots:
    void on_pushButton_clicked();
    void slot_requestFinished(bool bSuccess, const QString& strResult); //http请求结束
    void slot_mainshow();
signals:
    void start_window(int device_id,QString &baseUrl);
    void http_info();
    void signal_show();

private:
    Ui::Init_Dialog *ui;
    Dev_info_p Dev_T;
    cJSON *jsonroot;
    QString retCode;//错误代码
    QString retMsg;//错误信息
    int NUM;
    int device_idArry[4];
    Http_Manager *http;
    SQL_Api *SQL_object;
    Main_Widget *Main;
    void device_set(QString m_request);
};

#endif // INIT_DIALOG_H
