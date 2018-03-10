#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTextDocument>
#include <QTextCursor>
#include "sql_api.h"
#include "Identification.h"
#include "base_data.h"

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpClientSocket(QObject *parent=0);
signals:
    void updateClients(QString,int);
    void disconnected(int);
protected slots:
    void dataReceived();
    void slotDisconnected();
private:
    APNcmd m_APNcmd;
    SQL_Api SQL_object;
    Dev_info Mdev_info;
};

#endif // TCPCLIENTSOCKET_H
