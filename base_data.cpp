#include "base_data.h"
//#include <qDebug>

Base_Data::Base_Data(QObject *parent) :
    QObject(parent)
{
    apn_cmd = (APNcmd_p)malloc(sizeof(APNcmd));
    APN_CMD_Init();
}

Base_Data::~Base_Data()
{

}

void Base_Data::APN_CMD_Init(){

    memset(apn_cmd , 0, sizeof(APNcmd));
    apn_cmd->device_id=0;
    memset(apn_cmd->apnslist , 0, sizeof(apn_cmd->apnslist));
}

int  Base_Data::Resolve_JSON(const char *m_strResponse){\
    QString temp;
    temp.append(m_strResponse);
    //qDebug()<<"m_strResponse:"<<temp;
    strResponse = (char *)malloc(strlen(m_strResponse));
    memset(strResponse,0,strlen(m_strResponse));
    memcpy(strResponse,m_strResponse,strlen(m_strResponse));
    jsonroot = cJSON_Parse(strResponse);
    strcpy(retCode,  cJSON_GetObjectItem(jsonroot, "retCode")->valuestring);
    strcpy(retMsg,  cJSON_GetObjectItem(jsonroot, "retMsg")->valuestring);
    if(!strcmp(retCode,SUCCESS_CODE)){
        jsoncmd = cJSON_GetObjectItem(jsonroot, "cmd");
        apn_cmd->isConfirm = cJSON_GetObjectItem(jsoncmd, "isConfirm")->valueint;
        apn_cmd->isUsed = cJSON_GetObjectItem(jsoncmd, "isUsed")->valueint;
//        strcpy(apn_cmd->CreateTime, cJSON_GetObjectItem(jsoncmd, "createTime")->valuestring);
//        strcpy(apn_cmd->confimTime, cJSON_GetObjectItem(jsoncmd, "confirmTime")->valuestring);
//        strcpy(apn_cmd->stopTime, cJSON_GetObjectItem(jsoncmd, "stopTime")->valuestring);
//        id=2;
        apn_cmd->id = cJSON_GetObjectItem(jsoncmd, "id")->valueint;
        apn_cmd->cmdType = cJSON_GetObjectItem(jsoncmd, "cmdType")->valueint;
        strcpy(apn_cmd->startTime,  cJSON_GetObjectItem(jsoncmd, "startTime")->valuestring);
        strcpy(apn_cmd->endTime,  cJSON_GetObjectItem(jsoncmd, "endTime")->valuestring);
        apn_cmd->unitNum = cJSON_GetObjectItem(jsoncmd, "unitNum")->valueint;
        apn_cmd->loop = cJSON_GetObjectItem(jsoncmd, "loop")->valueint;
        apn_cmd->packetLen = cJSON_GetObjectItem(jsoncmd, "packetLen")->valueint;
        //设备信息
        jsondev = cJSON_GetObjectItem(jsoncmd,"device");
        apn_cmd->device_id = cJSON_GetObjectItem(jsondev,"id")->valueint;
        strcpy(apn_cmd->device_name,cJSON_GetObjectItem(jsondev,"name")->valuestring);
        //0
        strcpy(apn_cmd->dev_arry[0].unitPhone,cJSON_GetObjectItem(jsondev,"unit1Phone")->valuestring);
        apn_cmd->dev_arry[0].unitNet=cJSON_GetObjectItem(jsondev,"unit1Net")->valueint;
        apn_cmd->dev_arry[0].unitStatus=cJSON_GetObjectItem(jsondev,"unit1Status")->valueint;
        apn_cmd->dev_arry[0].unitTest=cJSON_GetObjectItem(jsondev,"unit1Test")->valueint;
        strcpy(apn_cmd->dev_arry[0].unitAPNList,cJSON_GetObjectItem(jsondev,"unit1APNList")->valuestring);
        //1
        strcpy(apn_cmd->dev_arry[1].unitPhone,cJSON_GetObjectItem(jsondev,"unit2Phone")->valuestring);
        apn_cmd->dev_arry[1].unitNet=cJSON_GetObjectItem(jsondev,"unit2Net")->valueint;
        apn_cmd->dev_arry[1].unitStatus=cJSON_GetObjectItem(jsondev,"unit2Status")->valueint;
        apn_cmd->dev_arry[1].unitTest=cJSON_GetObjectItem(jsondev,"unit2Test")->valueint;
        strcpy(apn_cmd->dev_arry[1].unitAPNList,cJSON_GetObjectItem(jsondev,"unit2APNList")->valuestring);
        //2
        strcpy(apn_cmd->dev_arry[2].unitPhone,cJSON_GetObjectItem(jsondev,"unit3Phone")->valuestring);
        apn_cmd->dev_arry[2].unitNet=cJSON_GetObjectItem(jsondev,"unit3Net")->valueint;
        apn_cmd->dev_arry[2].unitStatus=cJSON_GetObjectItem(jsondev,"unit3Status")->valueint;
        apn_cmd->dev_arry[2].unitTest=cJSON_GetObjectItem(jsondev,"unit3Test")->valueint;
        strcpy(apn_cmd->dev_arry[2].unitAPNList,cJSON_GetObjectItem(jsondev,"unit3APNList")->valuestring);
        //3
        strcpy(apn_cmd->dev_arry[3].unitPhone,cJSON_GetObjectItem(jsondev,"unit4Phone")->valuestring);
        apn_cmd->dev_arry[3].unitNet=cJSON_GetObjectItem(jsondev,"unit4Net")->valueint;
        apn_cmd->dev_arry[3].unitStatus=cJSON_GetObjectItem(jsondev,"unit4Status")->valueint;
        apn_cmd->dev_arry[3].unitTest=cJSON_GetObjectItem(jsondev,"unit4Test")->valueint;
        strcpy(apn_cmd->dev_arry[3].unitAPNList,cJSON_GetObjectItem(jsondev,"unit4APNList")->valuestring);
        //apn信息
        jsonapns = cJSON_GetObjectItem(jsoncmd, "apns");
        int i;
        for(i=0;cJSON_GetArrayItem(jsonapns,i)!=NULL;i++){
            apn_cmd->apnslist[i].id = cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "id")->valueint;
            strcpy(apn_cmd->apnslist[i].name,  cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "name")->valuestring);
            strcpy(apn_cmd->apnslist[i].apnName,  cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "apnName")->valuestring);
            strcpy(apn_cmd->apnslist[i].port,  cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "port")->valuestring);
            strcpy(apn_cmd->apnslist[i].mobileIP,  cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "mobileIP")->valuestring);
            strcpy(apn_cmd->apnslist[i].endIP,  cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "endIP")->valuestring);
            strcpy(apn_cmd->apnslist[i].exchangeIP,  cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "exchangeIP")->valuestring);
            apn_cmd->apnslist[i].netType = cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "netType")->valueint;
            apn_cmd->apnslist[i].apnType = cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "apnType")->valueint;
            apn_cmd->apnslist[i].addressType = cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "addressType")->valueint;
            apn_cmd->apnslist[i].pingType = cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "pingType")->valueint;
            strcpy(apn_cmd->apnslist[i].ggsnIP,  cJSON_GetObjectItem(cJSON_GetArrayItem(jsonapns,i), "ggsnIP")->valuestring);
            apn_cmd->apnslist[i].cmd_id = apn_cmd->id;
        }
        apn_cmd->APN_num = i;
    }else{
        apn_cmd->APN_num = 0;
    }
    return  apn_cmd->APN_num;
}

Ping_Data::Ping_Data(QObject *parent) :
    QObject(parent)
{

}

Ping_Data::~Ping_Data()
{

}


