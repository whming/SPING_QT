#include "init_dialog.h"
#include "ui_init_dialog.h"

Init_Dialog::Init_Dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Init_Dialog)
{
    ui->setupUi(this);
    NUM = 1;
    http = new Http_Manager();
    ui->lineEdit->setValidator(new QRegExpValidator(QRegExp(tr("[0-9]+")),this));
    ui->lineEdit_2->setValidator(new QRegExpValidator(QRegExp(tr("[0-9]+")),this));
    ui->lineEdit_3->setValidator(new QRegExpValidator(QRegExp(tr("[0-9]+")),this));
    ui->lineEdit_4->setValidator(new QRegExpValidator(QRegExp(tr("[0-9]+")),this));
    ui->lineEdit_6->setValidator(new QRegExpValidator(QRegExp(tr("[0-9]{0,3}\.[0-9]{0,3}\.[0-9]{0,3}\.[0-9]{0,3}\:[1-9][0-9]{0,3}")),this));
    SQL_object = new SQL_Api();
    Dev_T = (Dev_info_p)malloc(sizeof(Dev_info));
    memset(Dev_T,0,sizeof(Dev_info));
    connect(this,SIGNAL(http_info()),this,SLOT(on_pushButton_clicked()));
    connect(this,SIGNAL(signal_show()),this,SLOT(slot_mainshow()));
    connect(http,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                     this,SLOT(slot_requestFinished(bool,const QString&)));
    this->setLayout(ui->mainvLayout);

}

Init_Dialog::~Init_Dialog()
{
    if(NULL != Dev_T){
        free(Dev_T);
        Dev_T=NULL;
    }

    if(NULL != SQL_object){
        free(SQL_object);
        SQL_object=NULL;
    }

    delete ui;
}

void Init_Dialog::on_pushButton_clicked()
{
    QString strurl;
    QString strbody;

    if(!ui->lineEdit_6->text().isEmpty()){
        if(QRegExp(QString("[0-9]{0,3}\.[0-9]{0,3}\.[0-9]{0,3}\.[0-9]{0,3}\:[1-9][0-9]{0,8}")).exactMatch(ui->lineEdit_6->text()))
        {
           //qDebug()<<"成功"<<ui->lineEdit_6->text();
        }else{
           //qDebug()<<"失败"<<ui->lineEdit_6->text();
           QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>_服务器地址错误_</font>");
           message.setIconPixmap(QPixmap(":/images/error.png"));
           message.exec();
           return;
        }

    }else{
        QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>_服务器地址不能空_</font>");
        message.setIconPixmap(QPixmap(":/images/error.png"));
        message.exec();
        return;
    }


    if(!ui->lineEdit->text().isEmpty()){
        if(QRegExp(QString("^((1))\\d{10}$")).exactMatch(ui->lineEdit->text()))
        {
           //qDebug()<<"成功"<<ui->lineEdit->text();
        }else{
           //qDebug()<<"失败"<<ui->lineEdit->text();
           QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>1_手机号错误_</font>");
           message.setIconPixmap(QPixmap(":/images/error.png"));
           message.exec();
           return;
        }
    }else{
        QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>1_手机号不能空_</font>");
        message.setIconPixmap(QPixmap(":/images/error.png"));
        message.exec();
        return;
    }

    if(!ui->lineEdit_2->text().isEmpty()){
        if(QRegExp(QString("^((1))\\d{10}$")).exactMatch(ui->lineEdit_2->text()))
        {
           //qDebug()<<"成功"<<ui->lineEdit_2->text();
        }else{
           //qDebug()<<"失败"<<ui->lineEdit_2->text();
           QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>2_手机号错误_</font>");
           message.setIconPixmap(QPixmap(":/images/error.png"));
           message.exec();
           return;
        }
    }/*else{
        QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>2_手机号不能空_</font>");
        message.setIconPixmap(QPixmap(":/images/error.png"));
        message.exec();
        return;
    }*/

    if(!ui->lineEdit_3->text().isEmpty()){
        if(QRegExp(QString("^((1))\\d{10}$")).exactMatch(ui->lineEdit_3->text()))
        {
           //qDebug()<<"成功"<<ui->lineEdit_3->text();
        }else{
           //qDebug()<<"失败"<<ui->lineEdit_3->text();
           QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>3_手机号错误_</font>");
           message.setIconPixmap(QPixmap(":/images/error.png"));
           message.exec();
           return;
        }
    }/*else{
        QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>3_手机号不能空_</font>");
        message.setIconPixmap(QPixmap(":/images/error.png"));
        message.exec();
        return;
    }*/

    if(!ui->lineEdit_4->text().isEmpty()){
        if(QRegExp(QString("^((1))\\d{10}$")).exactMatch(ui->lineEdit_4->text()))
        {
           //qDebug()<<"成功"<<ui->lineEdit_4->text();
        }else{
           //qDebug()<<"失败"<<ui->lineEdit_4->text();
           QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>4_手机号错误_</font>");
           message.setIconPixmap(QPixmap(":/images/error.png"));
           message.exec();
           return;
        }
    }/*else{
        QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>4_手机号不能空_</font>");
        message.setIconPixmap(QPixmap(":/images/error.png"));
        message.exec();
        return;
    }*/

    strcpy(Dev_T->unitPhone,ui->lineEdit->text().toStdString().c_str());
    strcpy(Dev_T->unit1Phone,ui->lineEdit_2->text().toStdString().c_str());
    strcpy(Dev_T->unit2Phone,ui->lineEdit_3->text().toStdString().c_str());
    strcpy(Dev_T->unit3Phone,ui->lineEdit_4->text().toStdString().c_str());
    strurl.clear();
    strurl.append("http://");
    strurl.append(ui->lineEdit_6->text().toStdString().c_str());
    //qDebug()<<strurl;
    memset(Dev_T->server_addr,0,255);
    strcpy(Dev_T->server_addr,strurl.toStdString().c_str());

    strurl.append(DEVICE_ID);
    strbody.clear();
    strbody.append(DEVICE_ID_unitPhone);
    switch(NUM){
    case 1:
        strbody.append(ui->lineEdit->text().toStdString().c_str());

        break;
    case 2:
        strbody.append(ui->lineEdit_2->text().toStdString().c_str());

        break;
    case 3:
        strbody.append(ui->lineEdit_3->text().toStdString().c_str());

        break;
    case 4:
        strbody.append(ui->lineEdit_4->text().toStdString().c_str());

        break;
    }

    if(NUM<5){
//        HttpFun *Http = new HttpFun();
//         connect(Http,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
//                 this,SLOT(slot_requestFinished(bool,const QString&)));
         http->Post_Request(strurl,strbody);
    }

}



//http请求结束
void  Init_Dialog::slot_requestFinished(bool bSuccess, const QString& strResult)
{
    if(bSuccess)
    {
        ui->lineEdit_6->setDisabled(true);
        //qDebug()<<"chenggong";
        //qDebug()<<strResult;
        device_set(strResult);
        //qDebug()<<"retCode:"<<retCode <<"NUM"<<NUM;
        //qDebug()<<"retMsg:"<<retMsg;
        if(retCode == QString("0")){
            switch(NUM){
            case 1:
                device_idArry[0]=Dev_T->device_id;
                ui->lineEdit->setDisabled(true);
                break;
            case 2:
                if(device_idArry[0]==Dev_T->device_id){
                    ui->lineEdit_2->setDisabled(true);
                    device_idArry[1]=Dev_T->device_id;
                }
                else{
                    QString conect;
                    conect.append("<font color='red'>_前一个手机号不在同一设备_</font>");
                    QMessageBox message(QMessageBox::NoIcon, "错误", conect);
                    message.setIconPixmap(QPixmap(":/images/error.png"));
                    message.exec();
                    return;
                }

                break;
            case 3:
                if(device_idArry[0]==Dev_T->device_id){
                    ui->lineEdit_3->setDisabled(true);
                    device_idArry[2]=Dev_T->device_id;
                }
                else{
                    QString conect;
                    conect.append("<font color='red'>_前两个手机号不在同一设备_</font>");
                    QMessageBox message(QMessageBox::NoIcon, "错误", conect);
                    message.setIconPixmap(QPixmap(":/images/error.png"));
                    message.exec();
                    return;
                }

                break;
            case 4:
                if(device_idArry[0]==Dev_T->device_id){
                    ui->lineEdit_4->setDisabled(true);
                    device_idArry[2]=Dev_T->device_id;
                }
                else{
                    QString conect;
                    conect.append("<font color='red'>_前三个手机号不在同一设备_</font>");
                    QMessageBox message(QMessageBox::NoIcon, "错误", conect);
                    message.setIconPixmap(QPixmap(":/images/error.png"));
                    message.exec();
                    return;
                }
                NUM++;
//                Sql_Connect *sql = new Sql_Connect();
                SQL_object->Sql_data_open(DATABASE);
                SQL_object->Sql_data_instert_dev(Dev_T);
                SQL_object->Sql_data_close();

//                emit signal_show();

                 return;
                break;
            }
            NUM++;

        }else{
            QString conect;
            conect.sprintf("<font color='red'>_%d手机号不在系统内_</font>",NUM);
            QMessageBox message(QMessageBox::NoIcon, "错误", conect);
            message.setIconPixmap(QPixmap(":/images/error.png"));
            message.exec();
            return;
        }
        switch(NUM){
        case 1:
            break;
        case 2:
            if(!ui->lineEdit_2->text().isEmpty()){
                emit http_info();
            }
            break;
        case 3:

            if(!ui->lineEdit_3->text().isEmpty()){
                emit http_info();
            }
            break;
        case 4:

            if(!ui->lineEdit_4->text().isEmpty()){
                emit http_info();
            }
            break;
        }

        if(NUM<5){
            emit http_info();
        }
    }else{
        //qDebug()<<"shibai";
        QMessageBox message(QMessageBox::NoIcon, "错误", "<font color='red'>_服务器地址不正确_</font>");
        message.setIconPixmap(QPixmap(":/images/error.png"));
        message.exec();
    }
}



void Init_Dialog::device_set(QString m_request){
    retCode.clear();
    retMsg.clear();
    jsonroot = cJSON_Parse(m_request.toStdString().c_str());
    retCode.append(cJSON_GetObjectItem(jsonroot, "retCode")->valuestring);
    retMsg.append(cJSON_GetObjectItem(jsonroot, "retMsg")->valuestring);
    if(strcmp("0",cJSON_GetObjectItem(jsonroot, "retCode")->valuestring)==0)
        Dev_T->device_id = cJSON_GetObjectItem(jsonroot, "deviceId")->valueint;
}



void Init_Dialog::slot_mainshow(){
    SQL_Api SQL_object;
    Main = new Main_Widget();
    if(SQL_object.Sql_data_find_dev_info(DATABASE) > 0){
        Main->show();
        close();
    }else{
        if(NULL != Main){
            delete Main;
            Main = NULL;
        }
    }
}
