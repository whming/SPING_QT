#include "main_widget.h"
#include "ui_main_widget.h"
Main_Widget::Main_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Widget)
{
    ui->setupUi(this);
    unit = 0;
    Log_print = new Debug_Log(0+1);
    port =8010;
    http_tpe = -1;
    btn_status = true;
    cmd_data = new Base_Data();
    http = new Http_Manager();
    connect(http,SIGNAL(signal_requestFinished(bool,const QString&)),this,SLOT(slot_requestFinished_result(bool,const QString&)),Qt::DirectConnection);

    ping_data  = (PingData_P)malloc(sizeof(PingData));
//    data_init(ping_data);

    if(SQL_object.Sql_data_find_dev_info(DATABASE,&Mdev_info)>0)
        qDebug()<<"Sql_data_find_dev_info is success";
        else
        qDebug()<<"Sql_data_find_dev_info is failed";

    for(int i=0;i<4;i++){
        strUrl_CMD[i].clear();
        strbody_CMD[i].clear();
        strUrl_CMD[i].append(Mdev_info.server_addr);
        strUrl_CMD[i].append(APN_CMD);
        Log_print->PrintLog(LOG_INFO,"%d:%s,%s", 0+1,strUrl_CMD[i].toStdString().c_str(),"send cmd");
        strbody_CMD[i].append(APN_CONFIRM_unitPhone);
        switch(i){
            case 0:
                strbody_CMD[i].append(Mdev_info.unitPhone);
                break;
            case 1:
                strbody_CMD[i].append(Mdev_info.unit1Phone);
                break;
            case 2:
                strbody_CMD[i].append(Mdev_info.unit2Phone);
                break;
            case 3:
                strbody_CMD[i].append(Mdev_info.unit3Phone);
                break;
        }
       send_http_cmd(strUrl_CMD[i],strbody_CMD[i]);
       send_cmd_result_http("","");
    }
    webWidget = new QAxWidget;
    //设置ActiveX控件为IEMicrosoft Web Browser
    //设置ActiveX控件的id，最有效的方式就是使用UUID
    //此处的{8856F961-340A-11D0-A96B-00C04FD705A2}就是Microsoft Web Browser控件的UUID
    webWidget->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
    webWidget->setObjectName(QString::fromUtf8("webWidget"));//设置控件的名称
    webWidget->setFocusPolicy(Qt::StrongFocus);//设置控件接收键盘焦点的方式：鼠标单击、Tab键
    webWidget->setProperty("DisplayAlerts",false); //不显示任何警告信息。
    webWidget->setProperty("DisplayScrollBars",false); // 显示滚动条
    webUrl.clear();
    webUrl.append(Mdev_info.server_addr/*"http://47.94.150.126:9081"*//*BASE_URL*/);
    webUrl.append("/static/pad/?");
    webUrl.append(QString::number(Mdev_info.device_id)/*QString::number(6)*/);
    qDebug()<<"webUrl="<<webUrl;
    ui->bottomLayout->addWidget(webWidget);
    this->setLayout(ui->mainLayout);
    this->setWindowFlags(Qt::FramelessWindowHint);//无边框
    this->showFullScreen();

    on_pingBtn_3_clicked();
    on_webBtn_3_clicked();
    On_Start_Server();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_update_cmd()));
    timer->start(1000*60*30);
}

Main_Widget::~Main_Widget()
{

    delete ui;
}


void Main_Widget::slot_update_cmd(){
    for(int i=0;i<4;i++){
       send_http_cmd(strUrl_CMD[i],strbody_CMD[i]);
       send_cmd_result_http("","");
    }
}
void Main_Widget::send_http_cmd(const QString &strUrl,const QString &strBody){
    http_tpe=0;
    http->Post_Request(strUrl,strBody);
    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", 0+1,strUrl.toStdString().c_str(),"send Url");
    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", 0+1,strBody.toStdString().c_str(),"send body");
    Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s", 0+1,strUrl.toStdString().c_str(),"send Url");
    Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s", 0+1,strBody.toStdString().c_str(),"send body");
}

void Main_Widget::send_data_http(const QString &strUrl,const QString &strBody){
//qDebug()<<"strUrl"<<strUrl;
//qDebug()<<"strBody"<<strBody<<Unit_Num+1;
    http_tpe = 2;
    http->Post_Request(strUrl,strBody);
    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", 1,strUrl.toStdString().c_str(),"send strUrl");
    Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s",1,strUrl.toStdString().c_str(),"send strUrl");
    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", 1,strBody.toStdString().c_str(),"send data");
    Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s",1,strBody.toStdString().c_str(),"send data");
}

void Main_Widget::send_cmd_result_http(const QString &strUrl,const QString &strBody){
    QString strurl;
    QString strbody;
    http_tpe = 1;
    strurl.clear();
    strbody.clear();
    strurl.append(Mdev_info.server_addr);
    strurl.append(APN_CONFIRM);
    strbody.append(APN_CONFIRM_unitPhone);
    switch (cmd_data->apn_cmd->unitNum) {
        case 1:
            strbody.append(Mdev_info.unitPhone);
            break;
        case 2:
            strbody.append(Mdev_info.unit1Phone);
            break;
        case 3:
            strbody.append(Mdev_info.unit2Phone);
            break;
        case 4:
            strbody.append(Mdev_info.unit3Phone);
            break;
        default:
            break;
    }
//                    strbody.append(Unitphone);
    strbody.append(APN_CONFIRM_cmdId);
    strbody.append(QString::number(cmd_data->apn_cmd->id));
//    send_cmd_result_http(strurl,strbody);
    http->Post_Request(strurl,strbody);
    Log_print->SaveFileLog(LOG_INFO,"%d:%s", 1,"send cmd relust");

}

void Main_Widget::slot_requestFinished_result(bool bSuccess, const QString& strResult){
    QString text;

    QString strUrl;
    QString strbody;
    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", 1,strResult.toStdString().c_str(),"strResult");
    cmd_data->APN_CMD_Init();
    if(bSuccess)
    {
        switch(http_tpe){
            case 0:
                cmd_data->Resolve_JSON(strResult.toStdString().c_str());

                if(!strcmp(cmd_data->retCode,SUCCESS_CODE)){
//                    指令入库
                    text.clear();
                    text.append("cmd 获取成功");
                    SQL_object.Sql_data_open(DATABASE);
                    SQL_object.Sql_data_instert(cmd_data->apn_cmd);
                    SQL_object.Sql_data_close();
                    //        获取测试指令回馈
                }else{
                    text.clear();
                    text.append("cmd 获取失败");
                }
                break;
//            case 1:
//                Log_print->PrintLog(LOG_INFO,"%d:%s,%s", 1,strResult.toStdString().c_str(),"strResult");
//                Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s",1,strResult.toStdString().c_str(),"strResult");
//                text.clear();
//                if(strResult.indexOf(SUCCESS_CODE)!=-1){
//                    text.append("rel  return 成功");
//                }else{
//                    text.append("rel  return failed");
//                }
//                break;
            case 2:
                text.clear();
                if(strResult.indexOf(SUCCESS_CODE)!=-1){
                    text.append("data  return 成功");
                    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", 1,strResult.toStdString().c_str(),"send data sccuess");
                    Log_print->SaveFileLog(LOG_ERROR, "%d:%s",1,"send data sccuess");
                }else{
                    text.append("data  return failed");
                    Log_print->PrintLog(LOG_INFO,"%d:%s,%s", 1,strResult.toStdString().c_str(),"send data failed");
                    Log_print->SaveFileLog(LOG_INFO,"%d:%s",1,"send data failed");
                }
                break;
            default:
                break;
        }
    }else{
        Log_print->PrintLog(LOG_INFO,"%d:%s,%s", 1,strResult.toStdString().c_str(),"strResult");
        Log_print->SaveFileLog(LOG_INFO,"%d:%s,%s",1,strResult.toStdString().c_str(),"strResult");
    }
}


void Main_Widget::On_Start_Server()
{
    server = new Server(this,port);
    connect(server,SIGNAL(updateServer(QString,int)),this,SLOT(slot_ping_codec_text(QString,int)));
    connect(server,SIGNAL(updatecmd()),this,SLOT(slot_http_cmd()));
}

void Main_Widget::slot_http_cmd(){
    qDebug()<<"asdfasdf";
    for(int i=0;i<4;i++){
        //mutex.lock();
       send_http_cmd(strUrl_CMD[i],strbody_CMD[i]);
       send_cmd_result_http("","");
       // mutex.unlock();
        //Sleep(1000*5);
    }
}

void Main_Widget::loadNavigate()
{
  webWidget->dynamicCall("Navigate(const QString&)",webUrl);
//  webWidget->update();
}

void Main_Widget::ping_setHidden(bool status){
    ui->unit1Btn->setHidden(status);
    ui->unit2Btn->setHidden(status);
    ui->unit3Btn->setHidden(status);
    ui->unit4Btn->setHidden(status);
}

void Main_Widget::slot_ping_codec_text(const QString &ping_text, int m_Unit_Num){
    QString strurl;
    strurl.clear();
    QString strbody;
    strbody.clear();
    QString model_Num = ping_text.section("$",0,0);
    qDebug()<<model_Num;
    QString model_Text=ping_text;
    int model_Num_int = model_Num.toInt();
     if(model_Num_int==unit){
         model_Text.remove(0,2);
//         qDebug()<<"model_Text="<<model_Text<<"model_Num_int="<<model_Num_int;
         ui->Ping_code->append(model_Text);
         if(ping_text.indexOf("data") != -1){
                  qDebug()<<"ping_code 清除";
                  ui->Ping_code->clear();
         }
     }


}

void Main_Widget::on_webBtn_3_clicked()
{
    ui->webBtn_3->setStyleSheet(QStringLiteral("background-image: url(:/images/10.png);"));
    ui->pingBtn_3->setStyleSheet(QStringLiteral("background-image: url(:/images/2.png);"));
    if(!btn_status){
        ping_setHidden(true);
        ui->Ping_code->clear();
        unit=0;
        ui->Ping_code->setHidden(true);
        webWidget->setHidden(false);
        webWidget->update();
        btn_status = true;
    }
    ui->webBtn_3->setHidden(true);
    ui->pingBtn_3->setHidden(true);
    ui->webBtn_3->setHidden(false);
    ui->pingBtn_3->setHidden(false);
    loadNavigate();
}

void Main_Widget::on_pingBtn_3_clicked()
{
    ui->webBtn_3->setStyleSheet(QStringLiteral("background-image: url(:/images/1.png);"));
    ui->pingBtn_3->setStyleSheet(QStringLiteral("background-image: url(:/images/20.png);"));
    if(btn_status){
        webWidget->setHidden(true);
        if(unit ==0)
            unit = 1;
        ui->Ping_code->setHidden(false);
        ping_setHidden(false);
        btn_status = false;
    }
    ui->webBtn_3->setHidden(true);
    ui->pingBtn_3->setHidden(true);
    ui->webBtn_3->setHidden(false);
    ui->pingBtn_3->setHidden(false);
}

void Main_Widget::on_unit1Btn_clicked()
{

    ui->unit2Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c2.png);"));
    ui->unit3Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c3.png);"));
    ui->unit4Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c4.png);"));
    ui->unit1Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c10.png);"));
    ui->Ping_code->clear();
    unit = 1;
    ping_setHidden(true);
    ping_setHidden(false);
}

void Main_Widget::on_unit2Btn_clicked()
{

    ui->unit1Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c1.png);"));
    ui->unit3Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c3.png);"));
    ui->unit4Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c4.png);"));
    ui->unit2Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c20.png);"));
    ui->Ping_code->clear();
    unit = 2;
    ping_setHidden(true);
    ping_setHidden(false);
}

void Main_Widget::on_unit3Btn_clicked()
{

    ui->unit1Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c1.png);"));
    ui->unit4Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c4.png);"));
    ui->unit2Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c2.png);"));
    ui->unit3Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c30.png);"));
    ui->Ping_code->clear();
    unit = 3;
//    Sleep(1000);
    ping_setHidden(true);
    ping_setHidden(false);
}

void Main_Widget::on_unit4Btn_clicked()
{
    ui->unit1Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c1.png);"));
    ui->unit3Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c3.png);"));
    ui->unit2Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c2.png);"));
    ui->unit4Btn->setStyleSheet(QStringLiteral("background-image: url(:/images/c40.png);"));
    ui->Ping_code->clear();
    unit = 4;
    ping_setHidden(true);
    ping_setHidden(false);
}

//UPDATE cmd_table SET startTime = "00:00:00" WHERE startTime = "14:18:00";
//DELETE FROM cmd_table WHERE unitNum = 0;

