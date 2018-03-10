#ifndef BASE_DATA_H
#define BASE_DATA_H

#include <QObject>
#include "cJSON.h"
#include "Identification.h"
#include <string.h> //引用相关头文件

class Base_Data: public QObject
{
    Q_OBJECT

public:
    explicit Base_Data(QObject *parent = 0);
    ~Base_Data();
    int Resolve_JSON(const char *m_strResponse);
    APNcmd_p apn_cmd;
    void APN_CMD_Init();
    char retCode[6];//错误代码
    char retMsg[255];//错误信息
private:
    cJSON *jsonroot;
    cJSON *jsoncmd;
    cJSON *jsonapns;
    cJSON *jsondev;
    char  *strResponse;

};

class Ping_Data: public QObject
{
    Q_OBJECT

public:
    explicit Ping_Data(QObject *parent = 0);
    ~Ping_Data();

private:
    cJSON *jsonroot;
    cJSON *jsoncmd;
    cJSON *jsonapns;
    cJSON *jsondev;
    char  *strResponse;
    char retCode[6];//错误代码
    char retMsg[255];//错误信息
};

#endif // BASE_DATA_H
