#ifndef IDENTIFICATION_H
#define IDENTIFICATION_H
#include <QMutex>
//0	成功
#define SUCCESS_CODE "0"
//通过
#define PING_STATUS_CODE_S 0
//通过
#define PING_STATUS_CODE_D 1
//通过
#define PING_STATUS_CODE_M 2
//通过
#define PING_STATUS_CODE_E 3

#define PING_URL "/api/comm/result"

#define BASE_URL "http://47.94.150.126:9081"
//获取下发信息
#define APN_CMD "/api/comm/cmd"
//获取下发信息 参数
#define APN_CMD_unitPhone "unitPhone="
//确认下发信息
#define APN_CONFIRM "/api/comm/confirm"
//确认下发信息 参数
#define APN_CONFIRM_unitPhone "unitPhone="
//确认下发信息 参数
#define APN_CONFIRM_cmdId "&cmdId="

//#define DATABASE "./APN_data.db"
#define DATABASE "./APN_data.db"
#define HTTP_APNS_CMD  0
#define HTTP_APNS_CONFIRM  1

#define BUFSIZE 111141000
#define URLSIZE 1024


struct APN
{
    int id;
    int apn_id;
    char name[255];
    char apnName[255];
    char mobileIP[16];
    char endIP[16];
    char exchangeIP[16];
    int addressType;
    int netType;
    int apnType;
    char port[50];
    char ggsnIP[20];
    int pingType;
    int cmd_id;
    int unitNum;
};
typedef struct APN  APN_date,*APN_date_p;

typedef struct Device_info{
    char unitPhone[25];
    char unit1Phone[25];
    char unit2Phone[25];
    char unit3Phone[25];
    int device_id;
    int unitTest;
    int unitNet;
    int unitStatus;
    char server_addr[255];
    char unitAPNList[255];
}Dev_info,*Dev_info_p;

typedef struct apncmd{
    int id;
    int cmdType;
    char startTime[25];
    char endTime[25];
    int device_id;
    char device_name[255];
    int unitNum;
    int  loop;
    int  packetLen;
    int APN_num;
    int isConfirm;
    int isUsed;
    Dev_info dev_arry[4];
    APN_date apnslist[255];
}APNcmd,*APNcmd_p;



typedef struct Unit_info{
char unitPhone[25];
int unitNum;
}Unitinfo,*Unitinfo_p;


typedef struct PingData{
    char unitPhone[12];
    char resultSn[25];
    int cmdId;
    int cmdType;
    int apnId;

    int netType;
    int errType;
    int apnActivate;
    int ipIsGet;
    char ipaddr[255];

    int pingType;
    int pingMobileIP;
    char *pingMobileIPText;
    int pingEndIP;
    char *pingEndIPText;

    int pingExchangeIP;
    char *pingExchangeIPText;
    char startTime[25];
    char endTime[25];
    int serial;

    int singleEnd;
}PingData,*PingData_P;

#endif // IDENTIFICATION_H
