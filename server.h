#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include "tcpclientsocket.h"
#include "debug_log.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent=0,int port=0);
    QList<TcpClientSocket*> tcpClientSocketList;
    static int clientnum;//指定接收消息的客户端
    static int clientAllNum;//客户端总数
     static int deleteClient;//将移除的客户端
     static int addClient;//将增加的客户端
    static int getClientnum();
    static void setClientnum(int value);
    void sendMessageToClient(QString msg,int socketDescriptor);//发送信息到指定客户端
    void sendMessageToAllClient(QString msg);//发送信息到所有客户端

    static int getClientAllNum();
    static void setClientAllNum(int value);

    static int getDeleteClient();
    static void setDeleteClient(int value);

    static int getAddClient();
    static void setAddClient(int value);

signals:
    void updateServer(QString,int);
    void updatecmd();
public slots:
    void updateClients(QString,int);
    void slotDisconnected(int);
protected:
    void incomingConnection(int m_socketDescriptor);
private:
    QMutex mutex;

};

#endif // SERVER_H
