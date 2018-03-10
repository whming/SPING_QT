#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <windows.h>
#include <process.h>    /* _beginthread, _endthread */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <QSpacerItem>
#include <ActiveQt/QAxWidget>
#include <QTimer>
#include <QtConcurrent>
#include "server.h"
#include "http_manager.h"
#include "sql_api.h"
#include "Identification.h"
#include "base_data.h"


namespace Ui {
class Main_Widget;
}

class Main_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Widget(QWidget *parent = 0);
    ~Main_Widget();
signals:

private slots:
    void slot_ping_codec_text(const QString &ping_text, int m_Unit_Num);

    void on_webBtn_3_clicked();

    void on_pingBtn_3_clicked();

    void on_unit1Btn_clicked();

    void on_unit2Btn_clicked();

    void on_unit3Btn_clicked();

    void on_unit4Btn_clicked();

    void slot_http_cmd();

    void slot_requestFinished_result(bool bSuccess, const QString& strResult);

    void slot_update_cmd();

private:
    void send_http_cmd(const QString &strUrl,const QString &strBody);
    void send_data_http(const QString &strUrl,const QString &strBody);
    void send_cmd_result_http(const QString &strUrl,const QString &strBody);
    void loadNavigate();
    void On_Start_Server();
    void ping_setHidden(bool status);
    Ui::Main_Widget *ui;
    bool btn_status;
    QAxWidget *webWidget;
    QString webUrl;
    Server *server;
    int port;//指定端口号

    Http_Manager *http;
    Base_Data *cmd_data;
    Dev_info Mdev_info;
//    QString Unitphone;
    APNcmd m_APNcmd;
    SQL_Api SQL_object;
    PingData_P ping_data;
    PingData_P ping_data_http;
    QString strUrl;
    QString strbody;
    Debug_Log *Log_print;
    int  unit;
    QMutex mutex;
    QString strUrl_CMD[4];
    QString strbody_CMD[4];
    int http_tpe;// 0：命令请求，1：命令反馈，2：数据回传
    QTimer *timer;
};

#endif // MAIN_WIDGET_H
