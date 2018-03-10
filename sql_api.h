#ifndef SQL_API_H
#define SQL_API_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QString>
#include "Identification.h"
class SQL_Api : public QObject
{
    Q_OBJECT

public:
    explicit SQL_Api(QObject *parent = 0);
    ~ SQL_Api();
    int Sql_data_open(const char *db_name);
    int Sql_data_create_table();
    void Sql_data_close();
    int Sql_data_instert(APNcmd_p date);
    int Sql_data_instert_dev(Dev_info_p date);
    int Sql_data_update();
    int Sql_data_delete();
    int Sql_data_find_apn(int unitnum,APN_date_p apn_list,const char *db_name);
    int Sql_data_find_cmd(int unitnum,const char *db_name);
    int Sql_data_find_cmd(int unitnum,APNcmd_p m_apn_cmd_p,const char *db_name);
    int Sql_data_find_dev(const char *db_name);
    int Sql_data_find_dev_info(const char *db_name);
    int Sql_data_find_dev_info(const char *db_name,Dev_info_p data);
signals:

public slots:

private:
    QSqlDatabase database;
    QSqlQuery *query;
};

#endif // SQL_API_H
