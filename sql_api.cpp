#include "sql_api.h"

SQL_Api::SQL_Api(QObject *parent) :
    QObject(parent)
{
   // QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      database = QSqlDatabase::database("qt_sql_default_connection");
    else
      database = QSqlDatabase::addDatabase("QSQLITE");
}

SQL_Api::~SQL_Api(){


}

int SQL_Api::Sql_data_open(const char *db_name){

    database.setDatabaseName(db_name);
    //|看是否能正确打开
    if(!database.open())
       {
         qDebug()<<"error"<<(database.lastError()).text();
           return 0;
       }else{
        query = new QSqlQuery(database);
           return 1;
    }

}

int SQL_Api::Sql_data_create_table(){
return 0;
}

void SQL_Api::Sql_data_close(){

    if(database.isOpen()){
        database.close();
        if(NULL != query)
            delete query;
    }

}

int SQL_Api::Sql_data_instert_dev(Dev_info_p date){
//    //qDebug()<<db_name;
//    if(0==Sql_data_open(db_name))
//        return 0;
    //QSqlQuery
    QString sql;
//    sql.append("insert into device_info values(");
//    sql.append(QString::number(date->device_id));
//    sql.append(", unit1Phone=\"");
//    sql.append(date->unitPhone);
//    sql.append("\", unit2Phone=\"");
//    sql.append(date->unit1Phone);
//    sql.append("\", unit3Phone=\"");
//    sql.append(date->unit2Phone);
//    sql.append("\", unit4Phone=\"");
//    sql.append(date->unit3Phone);
//    sql.append("\", serverAddr=\"");
//    sql.append(date->server_addr);
//    sql.append("\" where id=1;");
    sql.append("insert into device_info values(");
    sql.append(QString::number(1));
    sql.append(",");
    sql.append(QString::number(date->device_id));
    sql.append(", \"");
    sql.append(date->unitPhone);
    sql.append("\", \"");
    sql.append(date->unit1Phone);
    sql.append("\", \"");
    sql.append(date->unit2Phone);
    sql.append("\", \"");
    sql.append(date->unit3Phone);
    sql.append("\", \"");
    sql.append(date->server_addr);
    sql.append("\" );");
    //qDebug()<<sql;
//    sql.append("update device_info set device_id=4, unit1Phone=\"\",unit2Phone=\"\", unit3Phone=\"18846190137\", unit4Phone=\"18846190137\", serverAddr=\"http://47.94.150.126:9081\" where id=1;");
    if(query->exec(sql)){
        //qDebug()<<"db_name";
    }
    //qDebug()<<"db_name2";
//    Sql_data_close();
    return 1;
}

int SQL_Api::Sql_data_instert(APNcmd_p date){
    QString cmdsql;
    QString devsql;
    QString apnsql;
    QString sql;
    int cmd_id=0;
    int dev_id=0;
    int apn_id=0;
    cmdsql.clear();
    cmdsql.append("select * from cmd_table;");
    devsql.clear();
    devsql.append("select * from device_table;");
    apnsql.clear();
    apnsql.append("select * from apn_table;");

    sql.clear();
    sql.append("insert into cmd_table values(");
    if(query->exec(cmdsql)){
        if(query->last())
        {
            cmd_id =query->value("id").toInt();
            cmd_id++;
        }else{
            cmd_id=1;
         }
    }else{
        cmd_id = 1;
    }
    sql.append(QString::number(cmd_id));
    sql.append(",");
    sql.append(QString::number(date->id));
    sql.append(",");
    if(query->exec(devsql)){
        if(query->last())
        {
            dev_id =query->value("id").toInt();
                dev_id++;
        }else{
           dev_id=1;
        }
    }else{
        dev_id=1;
    }
    sql.append(QString::number(dev_id));
    sql.append(",");
    sql.append(QString::number(date->unitNum));
    sql.append(",");
    sql.append("\"\"");
    sql.append(",");
    sql.append(QString::number(date->cmdType));
    sql.append(",\"");
    sql.append(date->startTime);
    sql.append("\",\"");
    sql.append(date->endTime);
    sql.append("\",");
    sql.append(QString::number(date->loop));
    sql.append(",");
    sql.append(QString::number(date->packetLen));
    sql.append(",");
    sql.append(QString::number(date->isConfirm));
    sql.append(",");
    sql.append(QString::number(date->isUsed));
//    sql.append(",");
//    sql.append(QString::number(date->APN_num));
    sql.append(");");
//    sql.append(",\"");
//    sql.append(date->CreateTime);
//    sql.append("\",\"");
//    sql.append(date->confimTime);
//    sql.append("\",\"");
//    sql.append(date->stopTime);
//    sql.append("\");");
    //执行sql
    if(query->exec(sql))
        qDebug()<<"cmd insert 1";
    else
        qDebug()<<"cmd insert 2";
    qDebug()<<"cmd insert:"<<sql;
//    query->exec(sql);
    sql.clear();
    sql.append("insert into device_table values(");
    sql.append(QString::number(dev_id));
    sql.append(",");
    sql.append(QString::number(date->device_id));
    sql.append(",\"");
    sql.append(date->device_name);
    sql.append("\",\"");
    sql.append(date->dev_arry[0].unitPhone);
    sql.append("\",");
    sql.append(QString::number(date->dev_arry[0].unitTest));
    sql.append(",");
    sql.append(QString::number(date->dev_arry[0].unitNet));
    sql.append(",");
    sql.append(QString::number(date->dev_arry[0].unitStatus));
    sql.append(",\"");
    sql.append(date->dev_arry[0].unitAPNList);

    sql.append("\",\"");
    sql.append(date->dev_arry[1].unitPhone);
    sql.append("\",");
    sql.append(QString::number(date->dev_arry[1].unitTest));
    sql.append(",");
    sql.append(QString::number(date->dev_arry[1].unitNet));
    sql.append(",");
    sql.append(QString::number(date->dev_arry[1].unitStatus));
    sql.append(",\"");
    sql.append(date->dev_arry[1].unitAPNList);
    sql.append("\",\"");

    sql.append(date->dev_arry[2].unitPhone);
    sql.append("\",");
    sql.append(QString::number(date->dev_arry[2].unitTest));
    sql.append(",");
    sql.append(QString::number(date->dev_arry[2].unitNet));
    sql.append(",");
    sql.append(QString::number(date->dev_arry[2].unitStatus));
    sql.append(",\"");
    sql.append(date->dev_arry[2].unitAPNList);
    sql.append("\",\"");

    sql.append(date->dev_arry[3].unitPhone);
    sql.append("\",");
    sql.append(QString::number(date->dev_arry[3].unitTest));
    sql.append(",");
    sql.append(QString::number(date->dev_arry[3].unitNet));
    sql.append(",");
    sql.append(QString::number(date->dev_arry[3].unitStatus));
    sql.append(",\"");
    sql.append(date->dev_arry[3].unitAPNList);
    sql.append("\");");
    if(query->exec(sql))
        qDebug()<<"dev insert 1";
    else
        qDebug()<<"dev insert 2";

    if(query->exec(apnsql)){
        if(query->last())
        {
            apn_id =query->value("id").toInt();
                apn_id++;
        }else{
           apn_id=1;
        }
    }else{
        apn_id=1;
    }

    for(int i=0;i < date->APN_num;i++){
        sql.clear();
        sql.append("insert into apn_table values(");

        sql.append(QString::number(apn_id++));
        sql.append(",");
        sql.append(QString::number(date->apnslist[i].id));
        sql.append(",\"");
        sql.append(date->apnslist[i].name);
        sql.append("\",\"");
        sql.append(date->apnslist[i].apnName);
        sql.append("\",\"");
        sql.append(date->apnslist[i].mobileIP);
        sql.append("\",\"");
        sql.append(date->apnslist[i].endIP);
        sql.append("\",\"");
        sql.append(date->apnslist[i].exchangeIP);
        sql.append("\",");
        sql.append(QString::number(date->apnslist[i].netType));
        sql.append(",");
        sql.append(QString::number(date->apnslist[i].pingType));
        sql.append(",");
        sql.append(QString::number(date->apnslist[i].addressType));
        sql.append(",");
        sql.append(QString::number(date->apnslist[i].apnType));
        sql.append(",\"");
        sql.append(date->apnslist[i].ggsnIP);
        sql.append("\",\"");
        sql.append(date->apnslist[i].port);
        sql.append("\",");
        sql.append(QString::number(/*date->apnslist[i].*/cmd_id));
        sql.append(");");
        qDebug()<<"sql:"<<sql;
        if(query->exec(sql))

            qDebug()<<"apn insert 1";
        else
            qDebug()<<"apn insert 2";

    }


    return 0;
}

int SQL_Api::Sql_data_update(){
return 0;
}

int SQL_Api::Sql_data_delete(){
return 0;
}

int SQL_Api::Sql_data_find_apn(int unitnum,APN_date_p apn_list,const char *db_name){

//    Sql_data_open(db_name);
    if(!database.isOpen()){
        Sql_data_open(db_name);
    }
    //QSqlQuery
    QString sql;
    int apn_num=0;
    int i=0;
    sql.append("select * from apn_table where cmd_id = ");
    sql.append(QString::number(unitnum));
    sql.append(";");
    if(unitnum <=0){
        Sql_data_close();
        return 0;
    }

//    qDebug()<<"apn:"<<sql;
    if(query->exec(sql)){
        query->last();

        apn_num=query->at()+1;

        if(apn_num>0){
            query->first();
            do
            {
                apn_list[i].id=query->value("id").toInt();
                apn_list[i].apn_id=query->value("apn_id").toInt();
                memset(apn_list[i].name,0,255);
                strcpy(apn_list[i].name,query->value("name").toString().toStdString().c_str());
                memset(apn_list[i].apnName,0,255);
                strcpy(apn_list[i].apnName,query->value("apnName").toString().toStdString().c_str());
                memset(apn_list[i].mobileIP,0,16);
                strcpy(apn_list[i].mobileIP,query->value("mobileIP").toString().toStdString().c_str());
                memset(apn_list[i].endIP,0,16);
                strcpy(apn_list[i].endIP,query->value("endIP").toString().toStdString().c_str());
                memset(apn_list[i].exchangeIP,0,16);
                strcpy(apn_list[i].exchangeIP,query->value("exchangeIP").toString().toStdString().c_str());

                apn_list[i].netType = query->value("netType").toInt();

                apn_list[i].pingType = query->value("pingType").toInt();

                apn_list[i].addressType = query->value("addressType").toInt();

                apn_list[i].apnType = query->value("apnType").toInt();
                memset(apn_list[i].ggsnIP,0,20);
                strcpy(apn_list[i].ggsnIP,query->value("ggsnIP").toString().toStdString().c_str());
                memset(apn_list[i].port,0,50);
                strcpy(apn_list[i].port,query->value("port").toString().toStdString().c_str());

    //            qDebug()<<"query->value(\"cmd_id\").toInt():" << query->value("cmd_id").toInt();

                apn_list[i].cmd_id = query->value("cmd_id").toInt();
                i++;
            }while(query->next());

        }

    }else{
        //qDebug()<<"2";
        return apn_num;
    }
    Sql_data_close();
return apn_num;
}


int SQL_Api::Sql_data_find_cmd(int unitnum,const char *db_name){
    //qDebug()<<db_name;
    Sql_data_open(db_name);
    //QSqlQuery
    QString sql;
    int cmd_id;
    sql.append("select id from cmd_table where unitNum = ");
    sql.append(QString::number(unitnum));
    sql.append(";");
    //qDebug()<<sql;
    if(query->exec(sql)){
        if(query->last())
        {
            cmd_id =query->value(0).toInt();
        }
    }else{
        cmd_id = 0;
    }

    //qDebug()<<"find cmd cmd_id:"<<cmd_id;
    Sql_data_close();

return cmd_id;
}

int SQL_Api::Sql_data_find_cmd(int unitnum,APNcmd_p m_apn_cmd_p,const char *db_name){
//    qDebug()<<db_name;
    Sql_data_open(db_name);
    //QSqlQuery
    QString sql;
    int cmd_id;
    sql.append("select * from cmd_table where unitNum = ");
    sql.append(QString::number(unitnum));
    sql.append(";");
//    qDebug()<<"cmd----sql::"<<sql;
    if(query->exec(sql)){
        if(query->last())
         {
            m_apn_cmd_p->id  = query->value("cmd_id").toInt();
//            qDebug()<<"m_apn_cmd_p->id" << m_apn_cmd_p->id;
            m_apn_cmd_p->device_id = query->value("device_id").toInt();
            m_apn_cmd_p->cmdType = query->value("cmdType").toInt();
            m_apn_cmd_p->loop = query->value("loop").toInt();
            m_apn_cmd_p->packetLen = query->value("packetLen").toInt();
            memset(m_apn_cmd_p->endTime,0,25);
            strcpy(m_apn_cmd_p->endTime,query->value("endTime").toString().toStdString().c_str());
            memset(m_apn_cmd_p->startTime,0,25);
            strcpy(m_apn_cmd_p->startTime,query->value("startTime").toString().toStdString().c_str());
            cmd_id=query->value("id").toInt();


         }
    }else{
        cmd_id =0;
    }
    Sql_data_close();

return cmd_id;
}


int SQL_Api::Sql_data_find_dev(const char *db_name){
        //qDebug()<<db_name;
        Sql_data_open(db_name);
        //QSqlQuery
        QString sql;
        int device_id;
        sql.append("select device_id from device_table;");
        //qDebug()<<sql;
        if(query->exec(sql)){
            if(query->last())
            {
                device_id =query->value(0).toInt();
            }
        }
        //qDebug()<<"find  device_id:"<<device_id;
        Sql_data_close();
        return device_id;
}


int SQL_Api::Sql_data_find_dev_info(const char *db_name){
    //qDebug()<<db_name;
    Sql_data_open(db_name);
    //QSqlQuery
    QString sql;
    int device_info_id;
    int sql_num=0;
    sql.append("select device_id from device_info;");
    //qDebug()<<sql;
    if(query->exec(sql)){
        if(query->last())
        {
            device_info_id =query->value(0).toInt();
        }
        //qDebug()<<"find  device_id:"<<device_info_id;
    }
//    query->last();
    sql_num = query->at()+1;
    //qDebug()<<"find  device_id:"<<sql_num;
    Sql_data_close();
    return sql_num;
}


int SQL_Api::Sql_data_find_dev_info(const char *db_name,Dev_info_p data){
    //qDebug()<<db_name;
    if(0==Sql_data_open(db_name)){
        return 0;
    }
    //QSqlQuery
    QString sql;
    sql.append("select * from device_info;");

    if(query->exec(sql)){
        if(query->last())
        {
            data->device_id =query->value("device_id").toInt();
            strcpy(data->server_addr,query->value("serverAddr").toString().toStdString().c_str());

            strcpy(data->unitPhone,query->value("unit1Phone").toString().toStdString().c_str());
            strcpy(data->unit1Phone,query->value("unit2Phone").toString().toStdString().c_str());
            strcpy(data->unit2Phone,query->value("unit3Phone").toString().toStdString().c_str());
            strcpy(data->unit3Phone,query->value("unit4Phone").toString().toStdString().c_str());
        }

    }
    Sql_data_close();
    return 1;
}

