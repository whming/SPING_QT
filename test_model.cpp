#include "test_model.h"
#include <string>
//QSerialPort *Unit_Serial;
QMutex mutex;
extern int  unit;
SQL_Api SQL_object;
Test_Model::Test_Model(int Serial_Num,QObject *parent) :
    QThread(parent)
{
    Unit_Num = Serial_Num;
    http_tpe = 0;

    sim_status = false;
    Net_Open_status = false;
    Unitphone.clear();
    ServerAddr.clear();
    serial_status = false;
    Unit_Serial = NULL;
    Log_print = new Debug_Log(Unit_Num+1);
}

Test_Model::~Test_Model(){
    if(NULL != Unit_Serial){
        delete Unit_Serial;
        Unit_Serial = NULL;
    }

    if( NULL != ping_data ){
       free(ping_data);
       ping_data = NULL;
    }

    if(Log_print != NULL){
       delete Log_print;
       Log_print = NULL;
    }
}

void Test_Model::Init(){
    QString text;
    int serial_num=0;
    cmd_data = new Base_Data();
    ping_data  = (PingData_P)malloc(sizeof(PingData));
    data_init(ping_data);

    SQL_object.Sql_data_find_dev_info(DATABASE,&Mdev_info);

    ServerAddr.append(Mdev_info.server_addr);
    if(unit == Unit_Num+1 )
    emit codec_append(ServerAddr, Unit_Num+1);
    Device_id = Mdev_info.device_id;
    if(unit == Unit_Num+1 )
    emit codec_append(QString::number(Device_id), Unit_Num+1);
    switch(Unit_Num){
        case 0:
            Unitphone.append(Mdev_info.unitPhone);
            break;
        case 1:
            Unitphone.append(Mdev_info.unit1Phone);
            break;
        case 2:
            Unitphone.append(Mdev_info.unit2Phone);
            break;
        case 3:
            Unitphone.append(Mdev_info.unit3Phone);
            break;
        default:
            break;
    }
    if(unit == Unit_Num+1 )
    emit codec_append(Unitphone, Unit_Num+1);
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QString temp = info.description();
        QTextDocument document;
        document.setHtml(temp);
        QTextCursor highlightCursor = document.find(QString("AT"), 0, QTextDocument::FindWholeWords);
        if (!highlightCursor.isNull())
        {
            if(serial_num == Unit_Num){
                Unit_Serial = new QSerialPort;
                Unit_Serial->setPort(info);
                break;
            }
            serial_num++;
        }
    }

    if(NULL != Unit_Serial){
        connect(Unit_Serial,&QSerialPort::readyRead,this,&Test_Model::/*Serial_inline_read*/Serial_read,Qt::DirectConnection);
    }
    connect(this,SIGNAL(write_signal(const QString &)),this,SLOT(Serial_write(const QString &)),Qt::BlockingQueuedConnection);

    connect(this,SIGNAL(http_cmd_post(const QString &,const QString &)),this,SLOT(send_cmd_http(const QString &,const QString &)),Qt::DirectConnection);
    connect(this,SIGNAL(http_data_post(const QString &,const QString &)),this,SLOT(send_data_http(const QString &,const QString &)),Qt::DirectConnection);
    http = new Http_Manager();
    connect(http,SIGNAL(signal_requestFinished(bool,const QString&)),this,SLOT(slot_requestFinished_result(bool,const QString&)),Qt::DirectConnection);

}

int Test_Model::IP_judge(char *IP,char *m_ggs_IP){
    char seg[] = "/"; /*分隔符这里为逗号comma，分隔符可以为你指定的，如分号，空格等*/
    int IP_4[4]={0};
    char *ip_t;
    char *substr;
    int i;
    ip_t= strtok(m_ggs_IP, seg);/*利用现成的分割函数,substr为分割出来的子字符串*/
//    printf("ip_t=%s\n",ip_t);
    char *m_count = strtok(NULL,seg);/*在第一次调用时，strtok()必需给予参数str字符串，*/
    if(m_count==NULL)
        return 0;
    printf("m_count=%s\n",m_count);
    IP_JS ip(ip_t,m_count);
    char seg1[] = "."; /*分隔符这里为逗号comma，分隔符可以为你指定的，如分号，空格等*/
    i =0;
    substr= strtok(IP, seg1);/*利用现成的分割函数,substr为分割出来的子字符串*/
    while (substr != NULL) {
        IP_4[i] = std::atoi( substr );/*把新分割出来的子字符串substr转为int拷贝到要存储的IP_4*/
        i++;
        printf("%s\n", substr);
        substr = strtok(NULL,seg1);/*在第一次调用时，strtok()必需给予参数str字符串，
 *                                  往后的调用则将参数str设置成NULL。每次调用成功则返回被分割出片段的指针。*/
    }
    if( IP_4[0] >= ip.netIP[0] && IP_4[0] >= ip.g_IP[0] ){
        if( IP_4[1] >= ip.netIP[1] && IP_4[1] >= ip.g_IP[1] ){
            if( IP_4[2] >= ip.netIP[2] && IP_4[2] >= ip.g_IP[2] ){
                return 1;
            }
        }
    }
    return 0;
}

bool Test_Model::Serial_open(){
    if(NULL == Unit_Serial){
        return false;
    }
    //打开串口
    if (!Unit_Serial->open(QIODevice::ReadWrite)){
        return false;
    }
    //设置波特率
    Unit_Serial->setBaudRate(115200);
    //设置数据位数
    Unit_Serial->setDataBits(QSerialPort::Data8);

    //设置奇偶校验
    Unit_Serial->setParity(QSerialPort::NoParity);

    //设置停止位
    Unit_Serial->setStopBits(QSerialPort::OneStop);

    //设置流控制
    Unit_Serial->setFlowControl(QSerialPort::NoFlowControl);
    //连接信号槽
//    connect(Unit_Serial,QSerialPort::readyRead,NULL,Test_Model::Serial_read,Qt::DirectConnection);
    return true;
}

bool Test_Model::Serial_close(){
    if (NULL == Unit_Serial)
    {
        return false;
    }
    Unit_Serial->close();
    return true;
}

bool Test_Model::Serial_write(const QString &write_data){
    if (NULL == Unit_Serial)
    {
        return false;
    }
//    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,write_data.toStdString().c_str(),"write_data");
    Unit_Serial->write(write_data.toStdString().c_str());
    return true;
}

QString Test_Model::Serial_read(){
    QString Serial_str;
    QByteArray Serial_buf;
    Serial_str.clear();
    if (NULL == Unit_Serial)
    {
        //qDebug() << "Serial read: error";
        return NULL;
    }
    Serial_buf = Unit_Serial->readAll();
    if (!Serial_buf.isEmpty())
    {
         Serial_str =  tr(Serial_buf);
    }else{
       emit loop_exit();
       return Serial_str;
    }

    if(unit == Unit_Num+1 )
        emit codec_append(Serial_str,Unit_Num+1);
    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,Serial_str.toStdString().c_str(),"Serial_str");
    //qDebug() << "Serial read: " << Serial_str<<"Unit_Num:"<<Unit_Num+1<<"ping_ip_status:"<<ping_ip_status;

    if(Serial_str.indexOf("+CGDCONT") != -1){
        emit loop_exit();
    }

    if(Serial_str.indexOf("+CSOCKSETPN") != -1){
        emit loop_exit();
    }

    if(Serial_str.indexOf("+CPIN: READY") != -1){
//        qDebug()<<"+CPIN:";
        sim_status=true;
        emit loop_exit();
    }else if(Serial_str.indexOf("SIM not inserted") != -1){
//        //qDebug()<<"SIM";
        sim_status=false;
        emit loop_exit();
    }

    if(Serial_str.indexOf("+CNSMOD: 0,8") != -1){
        net_status=0;
        emit loop_exit();
    }else if(Serial_str.indexOf("+CNSMOD") != -1){
        net_status=1;
        emit loop_exit();
    }

    if(Serial_str.indexOf("+NETOPEN: 0") != -1){
        Net_Open_status = true;
        emit loop_exit();
    }else if(Serial_str.indexOf("+NETOPEN:") != -1){
        Net_Open_status = false;
        emit loop_exit();
    }

    if(Serial_str.indexOf("+IPADDR:") != -1){
        if(NULL != ping_data){
            Serial_str.remove("\r\n+IPADDR: ").remove("\r\nOK\r\n").remove("\r\n").remove("AT+IPADDR").remove("+NETOPEN: 0").remove(" ");
            sprintf(ping_data->ipaddr,"%s",Serial_str.toStdString().c_str());
        }
        IP_status = true;
        emit loop_exit();
    }else if(Serial_str.indexOf("+IP ERROR:") != -1){
        IP_status = false;
        emit loop_exit();
    }

    if(Serial_str.indexOf("NETCLOSE") != -1){
        emit loop_exit();
    }else if(Serial_str.indexOf("+NETCLOSE: ") != -1){
        emit loop_exit();
    }

    if(Serial_str.indexOf("CPING")!=-1){
        if(Serial_str.indexOf("ERROR")!=-1){
            Serial_buf.clear();
            emit loop_exit();
            return Serial_str;
        }
        //qDebug() << "CPING设计: " << Serial_str<<"Unit_Num:"<<Unit_Num+1<<"ping_ip_status:"<<ping_ip_status;
        switch(ping_ip_status){
        case 1:
            pingMobileIPText.append(Serial_str);

            if(Serial_str.indexOf("+CPING: 1")!=-1){
                    ping_data->pingMobileIP=1;
            }

            if(Serial_str.indexOf("+CPING: 2")!=-1){
                    ping_data->pingMobileIP=0;
            }

            if(Serial_str.indexOf("+CPING: 3")!=-1){
//                qDebug() << "pingMobileIPText: " << pingMobileIPText<<"Unit_Num:"<<Unit_Num+1<<"ping_ip_status:"<<ping_ip_status;
                emit loop_exit();
            }
            break;
        case 2:
            pingEndIPText.append(Serial_str);
            if(Serial_str.indexOf("+CPING: 1")!=-1){
                    ping_data->pingEndIP=1;
            }

            if(Serial_str.indexOf("+CPING: 2")!=-1){
                    ping_data->pingEndIP=0;
            }

            if(Serial_str.indexOf("+CPING: 3")!=-1){
                //qDebug() << "pingEndIPText: " << pingEndIPText<<"Unit_Num:"<<Unit_Num+1<<"ping_ip_status:"<<ping_ip_status;
                emit loop_exit();
            }
            //
            break;
        case 3:
            pingExchangeIPText.append(Serial_str);
            if(Serial_str.indexOf("+CPING: 1")!=-1){
                    ping_data->pingExchangeIP=1;
            }

            if(Serial_str.indexOf("+CPING: 2")!=-1){
                    ping_data->pingExchangeIP=0;
            }

            if(Serial_str.indexOf("+CPING: 3")!=-1){
                //qDebug() << "pingExchangeIPText: " << pingExchangeIPText<<"Unit_Num:"<<Unit_Num+1<<"ping_ip_status:"<<ping_ip_status;
                emit loop_exit();
            }
            break;
            //
        }


    }

Serial_buf.clear();
    return Serial_str;
}



void Test_Model::send_cmd_http(const QString &strUrl,const QString &strBody){
//qDebug()<<"strUrl"<<strUrl;
//qDebug()<<"strBody"<<strBody;
//qDebug()<<"Unit_Num"<<Unit_Num;

    http_tpe = 0;
    http->Post_Request(strUrl,strBody);
    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,strBody.toStdString().c_str(),"send cmd");
    Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,strBody.toStdString().c_str(),"send cmd");
}



void Test_Model::send_cmd_result_http(const QString &strUrl,const QString &strBody){
//    //qDebug()<<"strUrl"<<strUrl;
//    //qDebug()<<"strBody"<<strBody;
    http_tpe = 1;
    http->Post_Request(strUrl,strBody);
    Log_print->SaveFileLog(LOG_INFO,"%d:%s", Unit_Num+1,"send cmd relust");
}



void Test_Model::send_data_http(const QString &strUrl,const QString &strBody){
//qDebug()<<"strUrl"<<strUrl;
//qDebug()<<"strBody"<<strBody<<Unit_Num+1;
    http_tpe = 2;
    http->Post_Request(strUrl,strBody);
    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,strBody.toStdString().c_str(),"send data");
    Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s",Unit_Num+1,strBody.toStdString().c_str(),"send data");
}




void Test_Model::slot_requestFinished_result(bool bSuccess, const QString& strResult){
    QString strUrl;
    QString strBody;
    QString text;
    cmd_data->APN_CMD_Init();
//    if(unit == Unit_Num+1 )
//        emit codec_append("text dd",Unit_Num+1);
    if(bSuccess)
    {
        switch(http_tpe){
            case 0:
                cmd_data->Resolve_JSON(strResult.toStdString().c_str());

                if(!strcmp(cmd_data->retCode,SUCCESS_CODE)){
//                    指令入库
                    text.clear();
                    text.append("cmd 获取成功");
                    if(unit == Unit_Num+1 )
                        emit codec_append(text,Unit_Num+1);
                    mutex.lock();
                    SQL_object.Sql_data_open(DATABASE);
                    SQL_object.Sql_data_instert(cmd_data->apn_cmd);
                    SQL_object.Sql_data_close();
                    mutex.unlock();
                    //        获取测试指令回馈

                }else{
                    text.clear();
                    text.append("cmd 获取失败");
                    if(unit == Unit_Num+1 )
                        emit codec_append(text,Unit_Num+1);

                }
                //emit loop_exit();
                break;
            case 1:
            Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,strResult.toStdString().c_str(),"strResult");
            Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s",Unit_Num+1,strResult.toStdString().c_str(),"strResult");
//                qDebug()<<strResult<<"\n"<<Unit_Num+1;
            text.clear();
           if(strResult.indexOf(SUCCESS_CODE)!=-1){
               text.append("rel  return 成功");
           }else{
               text.append("rel  return failed");
           }


           if(unit == Unit_Num+1 )
                emit codec_append(text,Unit_Num+1);
           //emit loop_exit();
                break;
            case 2:
            qDebug()<<strResult<<"\n"<<Unit_Num+1;
             text.clear();
            if(strResult.indexOf(SUCCESS_CODE)!=-1){
                text.append("data  return 成功");

                Log_print->SaveFileLog(LOG_ERROR, "%d:%s",Unit_Num+1,"send data sccuess");
            }else{
                text.append("data  return failed");
                Log_print->SaveFileLog(LOG_INFO,"%d:%s",Unit_Num+1,"send data failed");
            }

            if(unit == Unit_Num+1 )
                emit codec_append(text,Unit_Num+1);
                //qDebug()<<strResult;
            //emit loop_exit();

                break;
            default:
                break;
        }
    }else{
        //qDebug()<<"failed!";
//        Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,strResult.toStdString().c_str(),"strResult");
//        Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s",Unit_Num+1,strResult.toStdString().c_str(),"strResult");
    }
}


void Test_Model::data_clear(PingData_P tmp,APNcmd_p m_APNcmd_p){

     ////qDebug()<<"dddddd1";
     if( NULL != tmp->pingMobileIPText )
        memset( tmp->pingMobileIPText,0,2048);
    if( NULL != tmp->pingExchangeIPText )
        memset( tmp->pingExchangeIPText,0,2048);
    if( NULL != tmp->pingEndIPText )
        memset( ping_data->pingEndIPText,0,2048);
 ////qDebug()<<"dddddd1";
    memset( tmp->resultSn,0,25);
    memset( tmp->unitPhone,0,12);
    memset( tmp->startTime,0,25);
    memset( tmp->endTime,0,25);
    ////qDebug()<<"dddddd1";
    if(NULL != m_APNcmd_p){
        memset(m_APNcmd_p->endTime,0,9);
        memset(m_APNcmd_p->startTime,0,9);
    }
}



void Test_Model::data_init(PingData_P tmp){

    tmp->pingMobileIPText= (char *)malloc(2048);
    tmp->pingExchangeIPText = (char *)malloc(2048);
    tmp->pingEndIPText = (char *)malloc(2048);

    memset( tmp->pingMobileIPText,0,2048);
    memset( tmp->pingExchangeIPText,0,2048);
    memset( tmp->pingEndIPText,0,2048);

}


QString Test_Model::Serial_inline_read(){
    QString Serial_str;
    QByteArray Serial_buf;
RESTART_READ:
    Serial_str.clear();
    if (NULL == Unit_Serial)
    {
        return NULL;
    }
    Serial_buf = Unit_Serial->readAll();
    if (!Serial_buf.isEmpty())
    {
         Serial_str =  tr(Serial_buf);
    }else{
       Sleep(1000);
       goto RESTART_READ;
    }

    if(unit == Unit_Num+1 )
        emit codec_append(Serial_str,Unit_Num+1);
    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,Serial_str.toStdString().c_str(),"Serial_str");

    if(Serial_str.indexOf("+CPIN: READY") != -1){
        sim_status=true;
        emit loop_exit();
    }else if(Serial_str.indexOf("SIM not inserted") != -1){
        sim_status=false;
        emit loop_exit();
    }

    if(Serial_str.indexOf("+CNSMOD: 0,8") != -1){
        net_status=0;
        emit loop_exit();
    }else if(Serial_str.indexOf("+CNSMOD") != -1){
        net_status=1;
        emit loop_exit();
    }

    if(Serial_str.indexOf("+NETOPEN: 0") != -1){
        Net_Open_status = true;
        emit loop_exit();
    }else if(Serial_str.indexOf("+NETOPEN:") != -1){
        Net_Open_status = false;
        emit loop_exit();
    }

    Serial_buf.clear();
    return Serial_str;
}



void Test_Model::run(){
    setPriority(QThread::LowPriority);
    QString strUrl;
    QString strbody;
    QString AT_cmd;
    int m_cmd_id=0;
    int m_cmd_id_tmp=0;
    int apn_num;
    int ping_NUM_t[255];
    APN_date apn_list[255];
    QEventLoop loop;
    ping_ip_status = 1;
    ping_data  = (PingData_P)malloc(sizeof(PingData));
    data_init(ping_data);
    connect(this,SIGNAL(loop_exit()), &loop, SLOT(quit()));//超时信号
    serial_status=Serial_open();
RESTART_R:
    for(int j=0;j<255;j++){
        ping_NUM_t[j]=0;
    }
    while(1){
        Net_Open_status = false;
        strUrl.clear();
        strbody.clear();
//        获取测试指令
        strUrl.append(ServerAddr);
        strUrl.append(APN_CMD);
        strbody.append(APN_CONFIRM_unitPhone);
        strbody.append(Unitphone);
        emit http_cmd_post(strUrl,strbody);
        Sleep(1000*3);
        if(cmd_data->apn_cmd->id != 0){
            strUrl.clear();
            strbody.clear();
            strUrl.append(ServerAddr);
            strUrl.append(APN_CONFIRM);
            strbody.append(APN_CONFIRM_unitPhone);
            strbody.append(Unitphone);
            strbody.append(APN_CONFIRM_cmdId);
            strbody.append(QString::number(cmd_data->apn_cmd->id));
            send_cmd_result_http(strUrl,strbody);
            //loop.exec(); // 进入事件循环， 直到Serial_read信号发出， 这个语句才能退出
        }

        if(!serial_status){
            if(unit == Unit_Num+1 )
                 emit codec_append("串口打开失败", Unit_Num+1);
            serial_status=Serial_open();
            continue;
        }

        AT_cmd.clear();
        AT_cmd.append("AT+CPIN?\r\n");

        emit write_signal(AT_cmd);
        loop.exec(); // 进入事件循环， 直到Serial_read信号发出， 这个语句才能退出
        if(!sim_status){
            continue;
        }


        mutex.lock();
        m_cmd_id=SQL_object.Sql_data_find_cmd(Unit_Num+1,&m_APNcmd,DATABASE);
        mutex.unlock();
        if(m_cmd_id<=0){
            qDebug()<<"测试单元m_APNcmd.id失败"<<Unit_Num+1;
            qDebug()<<"测试单元m_cmd_id失败"<<m_cmd_id;
            Sleep(1000*5);
            continue;
        }

        if(m_APNcmd.cmdType==1&&m_cmd_id_tmp==m_APNcmd.id&&ping_NUM_t[0]==0){

            qDebug()<<"测试单元单点测试失败"<<Unit_Num+1;
            Sleep(1000*5);
            continue;
        }

        m_cmd_id_tmp = m_APNcmd.id;
        mutex.lock();
        apn_num = SQL_object.Sql_data_find_apn(m_cmd_id,apn_list,DATABASE);
        mutex.unlock();
        if(apn_num<=0){
            qDebug()<<"apn获取失败"<<Unit_Num+1;
            Sleep(1000*5);
            continue;
        }
        QTime newTime = QTime::currentTime();
        QTime startTime = QTime::fromString(m_APNcmd.startTime,"hh:mm:ss");
        QTime endTime = QTime::fromString(m_APNcmd.endTime,"hh:mm:ss");


            for(int i=0;i<apn_num;i++){
                if(m_APNcmd.cmdType==1){
                   if( ping_NUM_t[i]>m_APNcmd.loop||ping_NUM_t[i]==m_APNcmd.loop){
                       qDebug()<<"ping_NUM_t "<<ping_NUM_t[i]<<"m_APNcmd.loop "<<m_APNcmd.loop;
                       goto RESTART_R;
                   }
                }else{
                    if(newTime > startTime && newTime < endTime){

                    }else{
                        qDebug()<<"time out "<<Unit_Num+1;
                        goto RESTART_R;
                    }

                }

                AT_cmd.clear();
                AT_cmd.append("AT+CPIN?\r\n");
                emit write_signal(AT_cmd);
                loop.exec(); // 进入事件循环， 直到Serial_read信号发出， 这个语句才能退出


                if(!sim_status){
                    continue;
                }

                AT_cmd.clear();
                AT_cmd.append("AT+CNSMOD?\r\n");
                emit write_signal(AT_cmd);
                loop.exec(); // 进入事件循环， 直到Serial_read信号发出， 这个语句才能退出

                data_clear(ping_data,NULL);
                pingMobileIPText.clear();
                pingEndIPText.clear();
                pingExchangeIPText.clear();
                ping_NUM_t[i] = ping_NUM_t[i]+1;
                QDateTime current_date_time =QDateTime::currentDateTime();
                int current_date = current_date_time.toTime_t();
                strcpy(ping_data->startTime,QString::number(current_date).toStdString().c_str());
RESTART:
                AT_cmd.clear();
                AT_cmd.append("AT+CGDCONT=1,\"IP\",\"");
                AT_cmd.append(apn_list[i].apnName);
                AT_cmd.append("\"\r\n");
                emit write_signal(AT_cmd);
                //loop.exec();

                emit write_signal(QString(QLatin1String("AT+CSOCKSETPN=1\r\n")));
                //loop.exec();
                Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,"dddd","CSOCKSETPN");
                emit write_signal(QString(QLatin1String("AT+NETOPEN\r\n")));
                loop.exec();
                Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,"ddd","NETOPEN");
                if(!Net_Open_status){
                    emit write_signal(QString(QLatin1String("AT+NETCLOSE\r\n")));
                    loop.exec();
                    goto RESTART;
                }

                emit write_signal(QString(QLatin1String("AT+IPADDR\r\n")));
                loop.exec();

                emit write_signal(QString(QLatin1String("AT+IPADDR\r\n")));
                loop.exec();

                /*                if(!IP_status){
                    emit write_signal(QString(QLatin1String("AT+NETCLOSE\r\n")));
                    loop.exec(); // 进入事件循环， 直到Serial_read信号发出， 这个语句才能退出

                    goto RESTART;
                }*/

                if(IP_judge(ping_data->ipaddr,m_APNcmd.apnslist[i].ggsnIP)){
                    ping_data->ipIsGet = 0;
                }else{
                    ping_data->ipIsGet = 1;
                }


                for(int ip_num=0;ip_num<=3;ip_num++){
                    if(pingMobileIPText.isEmpty()&&ip_num==1)
                        ip_num=0;
                    if(pingEndIPText.isEmpty()&&ip_num==2)
                        ip_num=1;
                    if(pingExchangeIPText.isEmpty()){
                        if(ip_num==3)
                            ip_num=2;
                    }else{
                        break;
                    }

                    Sleep(1000*10);
                    AT_cmd.clear();
                    AT_cmd.append("AT+CPING=\"");
                    switch (ip_num) {
                    case 0:
                        AT_cmd.append(apn_list[i].mobileIP);
                        break;
                    case 1:
                        AT_cmd.append(apn_list[i].endIP);
                        break;
                    case 2:
                        AT_cmd.append(apn_list[i].exchangeIP);
                        break;
                    default:
                        break;
                    }

                    if(m_APNcmd.packetLen){
                        AT_cmd.append("\",1,");
                        AT_cmd.append(QString::number(m_APNcmd.packetLen));
                        AT_cmd.append(",64,1000,10000,255\r\n");
                    }else{
                        AT_cmd.append("\",1,4,64,1000,10000,255\r\n");
                    }

                    ping_ip_status = ip_num+1;
                    emit write_signal(AT_cmd);
                    loop.exec(); // 进入事件循环， 直到Serial_read信号发出， 这个语句才能退出
                    Sleep(1000*30);

                }





                /*Sleep(1000*10);

                AT_cmd.clear();
                AT_cmd.append("AT+CPING=\"");
                AT_cmd.append(apn_list[i].endIP);
                if(m_APNcmd.packetLen){
                    AT_cmd.append("\",1,");
                    AT_cmd.append(QString::number(m_APNcmd.packetLen));
                    AT_cmd.append(",64,1000,10000,255\r\n");
                }else{
                    AT_cmd.append("\",1,4,64,1000,10000,255\r\n");
                }
                 ping_ip_status = 2;
                emit write_signal(AT_cmd);
                loop.exec(); // 进入事件循环， 直到Serial_read信号发出， 这个语句才能退出


                //Sleep(1000*10);

                AT_cmd.clear();
                AT_cmd.append("AT+CPING=\"");
                AT_cmd.append(apn_list[i].exchangeIP);
                if(m_APNcmd.packetLen){
                    AT_cmd.append("\",1,");
                    AT_cmd.append(QString::number(m_APNcmd.packetLen));
                    AT_cmd.append(",64,1000,10000,255\r\n");
                }else{
                    AT_cmd.append("\",1,4,64,1000,10000,255\r\n");
                }
                 ping_ip_status = 3;
                emit write_signal(AT_cmd);
                loop.exec(); // 进入事件循环， 直到Serial_read信号发出， 这个语句才能退出*/





                AT_cmd.clear();
                emit write_signal(QString(QLatin1String("AT+NETCLOSE\r\n")));
                loop.exec(); // 进入事件循环， 直到Serial_read信号发出， 这个语句才能退出
                Sleep(1000*5);

                QDateTime endcurrent_date_time =QDateTime::currentDateTime();
                int endcurrent_date = endcurrent_date_time.toTime_t();

                QString end_time_str = QString::number(endcurrent_date);

                QString sn_date =endcurrent_date_time.toString("yyyyMMdd");
                strcpy(ping_data->endTime,end_time_str.toStdString().c_str());
                ping_data->ipIsGet=1;
                sprintf(ping_data->resultSn,"%03d%d%s%08d",Mdev_info.device_id,Unit_Num+1,sn_date.toStdString().c_str(),ping_NUM_t[i]);
//                qDebug()<<"resultSn"<<ping_data->resultSn;

                Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,pingExchangeIPText.toStdString().c_str(),"pingExchangeIPText");
                Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s",Unit_Num+1,pingExchangeIPText.toStdString().c_str(),"pingExchangeIPText");
                if(!pingExchangeIPText.isEmpty())
                    strcpy(ping_data->pingExchangeIPText,pingExchangeIPText.toStdString().c_str());
                else
                    continue;
                Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,pingMobileIPText.toStdString().c_str(),"pingMobileIPText");
                Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s",Unit_Num+1,pingMobileIPText.toStdString().c_str(),"pingMobileIPText");
                if(!pingMobileIPText.isEmpty())
                    strcpy(ping_data->pingMobileIPText,pingMobileIPText.toStdString().c_str());
                else
                    continue;
                Log_print->PrintLog(LOG_INFO,"%d:%s,%s", Unit_Num+1,pingEndIPText.toStdString().c_str(),"pingEndIPText");
                Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s",Unit_Num+1,pingEndIPText.toStdString().c_str(),"pingEndIPText");
                if(!pingEndIPText.isEmpty())
                    strcpy(ping_data->pingEndIPText,pingEndIPText.toStdString().c_str());
                else
                    continue;

                if(ping_data->pingMobileIP){
                    ping_data->apnActivate=1;
                }else{
                    if(ping_data->pingEndIP&&ping_data->pingExchangeIP){
                        ping_data->apnActivate=1;
                        ping_data->errType = 0;
                    }else{
                        ping_data->apnActivate=0;
                        if(net_status==apn_list[i].netType){
                            ping_data->errType = 1;
                        }else{
                            ping_data->errType = 2;
                        }
                    }
                }

                ping_data->apnId = apn_list[i].apn_id;
                ping_data->cmdId = m_APNcmd.id;
                ping_data->netType = apn_list[i].netType;
                ping_data->pingType = apn_list[i].pingType;
                strUrl.clear();
                strbody.clear();
                strUrl.append(ServerAddr);
                strUrl.append(PING_URL);
                strbody.append("unitPhone=");
                strbody.append(Unitphone);
                strbody.append("&resultSn=");
                strbody.append(QString(ping_data->resultSn));
                strbody.append("&cmdId=");
                strbody.append(QString::number(ping_data->cmdId));
                strbody.append("&cmdType=");
                strbody.append(QString::number(m_APNcmd.cmdType));
                strbody.append("&apnId=");
                strbody.append(QString::number(ping_data->apnId));
                strbody.append("&netType=");
                strbody.append(QString::number(ping_data->netType));
                strbody.append("&errType=");
                strbody.append(QString::number(ping_data->errType));
                strbody.append("&apnActivate=");
                strbody.append(QString::number(ping_data->apnActivate));
                strbody.append("&ipIsGet=");
                strbody.append(QString::number(ping_data->ipIsGet));
                strbody.append("&ipaddr=");
                strbody.append(ping_data->ipaddr);
                strbody.append("&pingType=");
                strbody.append(QString::number(ping_data->pingType));
                strbody.append("&pingMobileIP=");
                strbody.append(QString::number(ping_data->pingMobileIP));
                strbody.append("&pingMobileIPText=");
                strbody.append(ping_data->pingMobileIPText);
                strbody.append("&pingEndIP=");
                strbody.append(QString::number(ping_data->pingEndIP));
                strbody.append("&pingEndIPText=");
                strbody.append(ping_data->pingEndIPText);
                strbody.append("&pingExchangeIP=");
                strbody.append(QString::number(ping_data->pingExchangeIP));
                strbody.append("&pingExchangeIPText=");
                strbody.append(ping_data->pingExchangeIPText);
                strbody.append("&startTime=");
                strbody.append(ping_data->startTime);
                strbody.append("&endTime=");
                strbody.append(ping_data->endTime);
//                qDebug()<<"strbody"<<strbody;

                emit http_data_post(strUrl,strbody);
                Sleep(1000*3);
            }

    }

}
