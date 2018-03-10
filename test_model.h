#ifndef TEST_MODEL_H
#define TEST_MODEL_H

#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTextDocument>
#include <QDateTime>
#include <QTime>
#include <QObject>
#include <QTextCursor>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "http_manager.h"
#include "base_data.h"
#include "sql_api.h"
#include "ipjs.h"
#include "debug_log.h"

class Test_Model : public QThread
{
    Q_OBJECT

public:
    explicit Test_Model(int Serial_Num,QObject *parent = 0);
    ~Test_Model();
    bool Serial_open();
    bool Serial_close();
    void Init();
    QString ServerAddr;
    int Device_id;
    bool serial_status;
signals:
    void write_signal(const QString &read_str);
    void http_data_post(const QString &strUrl,const QString &data);
    void http_cmd_post(const QString &strUrl,const QString &data);
    void loop_exit();
    void codec_append(const QString &ping_text, int m_Unit_Num);
public slots:
    QString Serial_read();
    QString Serial_inline_read();
    bool Serial_write(const QString &write_data);
    void send_cmd_http(const QString &strUrl,const QString &strBody);
    void send_cmd_result_http(const QString &strUrl,const QString &strBody);
    void send_data_http(const QString &strUrl,const QString &strBody);
    void slot_requestFinished_result(bool bSuccess, const QString& strResult);
private:
    QSerialPort *Unit_Serial;
    virtual void run() Q_DECL_OVERRIDE;
    int IP_judge(char *IP,char *m_ggs_IP);
    int Unit_Num;
    Http_Manager *http;
    Base_Data *cmd_data;
//    SQL_Api *SQL_object;
    Dev_info Mdev_info;
    QString Unitphone;
//    char *ip_t;
//    char *substr;
//    int i;
//    int IP_4[4];
    APNcmd m_APNcmd;
    bool sim_status;
    int net_status;
    bool IP_status;
    bool Net_Open_status;
    PingData_P ping_data;
    PingData_P ping_data_http;

    QString pingMobileIPText;
    QString pingEndIPText;
    QString pingExchangeIPText;

    QString pingMobileIPText2;
    QString pingEndIPText2;
    QString pingExchangeIPText2;

    QString pingMobileIPText3;
    QString pingEndIPText3;
    QString pingExchangeIPText3;

    QString pingMobileIPText4;
    QString pingEndIPText4;
    QString pingExchangeIPText4;

    int ping_ip_status;
    void data_clear(PingData_P tmp,APNcmd_p m_APNcmd_p);
    void data_init(PingData_P tmp);

    Debug_Log *Log_print;
    int http_tpe;// 0：命令请求，1：命令反馈，2：数据回传
};

#endif // TEST_MODEL_H
