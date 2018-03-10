#include "server.h"
#include<QMessageBox>
#include<QDebug>
Server::Server(QObject *parent,int port)
    :QTcpServer(parent)
{
    QHostAddress serverIP;
    serverIP.setAddress("172.0.0.1");
//    listen(QHostAddress::Any,port);
    listen(QHostAddress::LocalHost,port);

}

int Server::getAddClient()
{
    return addClient;
}

void Server::setAddClient(int value)
{
    addClient = value;
}

int Server::getDeleteClient()
{
    return deleteClient;
}

void Server::setDeleteClient(int value)
{
    deleteClient = value;
}

int Server::getClientAllNum()
{
    return clientAllNum;
}

void Server::setClientAllNum(int value)
{
    clientAllNum = value;
}
int Server::clientnum=0;
int Server::clientAllNum=0;
int Server::deleteClient=0;
int Server::addClient=0;
int Server::getClientnum()
{
    return clientnum;
}

void Server::setClientnum(int value)
{
    clientnum = value;
}
//发送信息到客户端
void Server::sendMessageToClient(QString msg,int socketDescriptor)
{
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        if(socketDescriptor==tcpClientSocketList.at(i)->socketDescriptor()){
            QTcpSocket *item = tcpClientSocketList.at(i);
            QByteArray datasend =msg.toLocal8Bit();
            item->write(datasend);
            break;
        }

    }

}
//发送信息到所有客户端
void Server::sendMessageToAllClient(QString msg)
{
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        QByteArray datasend =msg.toLocal8Bit();
        item->write(datasend);
    }
}

void Server::incomingConnection(int socketDescriptor)
{
    TcpClientSocket *tcpClientSocket=new TcpClientSocket(this);
    connect(tcpClientSocket,SIGNAL(updateClients(QString,int)),this,SLOT(updateClients(QString,int)));
    connect(tcpClientSocket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);

    tcpClientSocketList.append(tcpClientSocket);
}

void Server::updateClients(QString msg,int length)
{
    qDebug()<<"msgupdate1"<<msg;
//    emit updatecmd();
    emit updateServer(msg,/*msg.length()*/length);

}
//客户端断开连接时删除
void Server::slotDisconnected(int descriptor)
{
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
            tcpClientSocketList.removeAt(i);
            deleteClient=i;
            return;
        }
    }
    return;
}
