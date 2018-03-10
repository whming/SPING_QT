#include "tcpclientsocket.h"
#include<QDebug>
TcpClientSocket::TcpClientSocket(QObject *parent)
{
    if(SQL_object.Sql_data_find_dev_info(DATABASE,&Mdev_info)>0)
        qDebug()<<"Sql_data_find_dev_info is success";
        else
        qDebug()<<"Sql_data_find_dev_info is failed";
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));

}


void TcpClientSocket::dataReceived()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = readAll();
    QString str =  QString::fromLocal8Bit(buffer);
    //刷新显示
    qDebug()<<str;
    if(!buffer.isEmpty())
    {
        QString str =  QString::fromLocal8Bit(buffer);
        //刷新显示
        QString model_Num = str.section("$",0,0);
        qDebug()<<model_Num;
        int model_Num_int = model_Num.toInt();
        QString comList;
        int serial_num=0;
        comList.clear();
        if(str.indexOf("com") != -1){
            foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
            {
                QString temp = info.description();
                QTextDocument document;
                document.setHtml(temp);
                QTextCursor highlightCursor = document.find(QString("AT"), 0, QTextDocument::FindWholeWords);

                if (!highlightCursor.isNull())
                {
                    /*if(serial_num == Unit_Num){
                        Unit_Serial = new QSerialPort;
                        Unit_Serial->setPort(info);
                        break;
                    }*/
                    comList.append(info.portName());
                    comList.append(",");
                    serial_num++;
                }
            }
            comList.append(QString::number(serial_num));
            comList.append("\n");
            QByteArray datasend =comList.toLocal8Bit();
            write(datasend);
//            sendMessageToClient(comList,length);
        }else if(str.indexOf("$cmd") != -1){
//            QString temp = "12$1,hhyh4g.nx,0,0,10.231.16.32/28,10.224.114.73,10.224.114.74,10.223.138.98,1,0,18295796475,6,29,121,0,4,10:49:00,20:49:22&2,GYGAJWT.NX,0,0,172.16.120.0/21,10.224.114.125,10.224.114.126,10.223.138.98,1,0,18295796475,6,34,121,0,4,10:49:00,20:49:22&3,WJ4GSPJK.nx,0,0,172.19.0.0/23,10.223.87.69,10.223.87.70,10.223.138.98,1,0,18295796475,6,56,121,0,4,10:49:00,20:49:22&4,NXYDJWT.NX,0,0,172.16.56.0/21,10.224.114.105,10.224.114.106,10.223.138.98,1,0,18295796475,6,58,121,0,4,10:49:00,20:49:22&5,nxczt4g.nx,0,0,172.30.0.0/22,10.223.87.97,10.223.87.98,10.223.138.98,1,0,18295796475,6,60,121,0,4,10:49:00,20:49:22&6,nxwzzf.nx,0,0,172.30.16.0/20,10.224.114.57,10.224.114.58,10.223.138.98,1,0,18295796475,6,62,121,0,4,10:49:00,20:49:22&7,xtwxxn.nx,0,0,10.236.0.0/19,10.224.114.141,10.224.114.142,10.223.138.98,1,0,18295796475,6,63,121,0,4,10:49:00,20:49:22&8,ZFWZZ.NX,0,0,10.232.0.0/14,10.224.114.149,10.223.138.98,10.223.138.98,1,0,18295796475,6,64,121,0,4,10:49:00,20:49:22&9,GYGA350M.NX,0,0,172.17.4.16/28,10.224.114.133,10.223.138.98,10.223.138.98,1,0,18295796475,6,65,121,0,4,10:49:00,20:49:22&10,jwtzw.nx,0,0,172.16.3.73,172.16.3.74,10.224.114.89,10.223.138.98,10.223.138.98,1,52,18295796475,6,66,121,0,4,10:49:00,20:49:22&11,CMMTM,0,0,10.237.0.0/27,61.135.169.125,61.135.169.125,61.135.169.125,1,0,18295796475,6,67,121,0,4,10:49:00,20:49:22&12,CMIOT,0,0,10.237.0.0/27,61.135.169.125,61.135.169.125,61.135.169.125,1,0,18295796475,6,68,121,0,4,10:49:00,20:49:22\n";
//             qDebug()<<temp;

            APN_date apn_list[255];

            int m_cmd_id=SQL_object.Sql_data_find_cmd(model_Num_int,&m_APNcmd,DATABASE);

            if(m_cmd_id<=0){
            qDebug()<<"测试单元m_APNcmd.id失败"<<model_Num_int;
            qDebug()<<"测试单元m_cmd_id失败"<<m_cmd_id;
            }else
            qDebug()<<"m_cmd_id="<<m_cmd_id<<"model_Num_int="<<model_Num_int;
            int apn_num = SQL_object.Sql_data_find_apn(m_cmd_id,apn_list,DATABASE);
            if(apn_num<=0){
            qDebug()<<"apn获取失败"<<model_Num_int;
            }else{
            qDebug()<<"apn获取成功"<<model_Num_int;
            }

            QString cmd_list;
            cmd_list.clear();
            cmd_list.append(QString::number(apn_num));
            cmd_list.append("$");

            for(int i=1;i<=apn_num;i++){
                cmd_list.append(QString::number(i));
                cmd_list.append("-");
                cmd_list.append(apn_list[i-1].apnName);
                cmd_list.append("-");
                //             cmd_list.append(apn_list[i-1].name);
                //             cmd_list.append("-");
                cmd_list.append(QString::number(apn_list[i-1].apnType));
                cmd_list.append("-");
                cmd_list.append(QString::number(apn_list[i-1].addressType));
                //             cmd_list.append("-");
                //             cmd_list.append(QString::number(apn_list[i-1].apnType));
                cmd_list.append("-");
                cmd_list.append(apn_list[i-1].ggsnIP);
                cmd_list.append("-");
                cmd_list.append(apn_list[i-1].mobileIP);
                cmd_list.append("-");
                cmd_list.append(apn_list[i-1].endIP);
                cmd_list.append("-");
                cmd_list.append(apn_list[i-1].exchangeIP);
                cmd_list.append("-");
                cmd_list.append(QString::number(apn_list[i-1].netType));
                cmd_list.append("-");
                cmd_list.append(QString::number(apn_list[i-1].pingType));
                cmd_list.append("-");
                switch(model_Num_int){
                    case 1:
                      cmd_list.append(Mdev_info.unitPhone);
                    break;
                    case 2:
                      cmd_list.append(Mdev_info.unit1Phone);
                    break;
                    case 3:
                      cmd_list.append(Mdev_info.unit2Phone);
                    break;
                    case 4:
                      cmd_list.append(Mdev_info.unit3Phone);
                    break;
                }
                cmd_list.append("-");
                cmd_list.append(QString::number(Mdev_info.device_id));
                cmd_list.append("-");
                cmd_list.append(QString::number(apn_list[i-1].apn_id));
                cmd_list.append("-");
                cmd_list.append(QString::number(apn_list[i-1].cmd_id));
                cmd_list.append("-");
                cmd_list.append(QString::number(m_APNcmd.cmdType));
                cmd_list.append("-");
                cmd_list.append(QString::number(m_APNcmd.packetLen));
                cmd_list.append("-");
                cmd_list.append(m_APNcmd.startTime);
                cmd_list.append("-");
                cmd_list.append(m_APNcmd.endTime);
                cmd_list.append("&");
            }
            qDebug()<<"apn获取成功 info="<<cmd_list;
            cmd_list.append("\n");
            QByteArray datasend =cmd_list.toLocal8Bit();
             write(datasend);
        } else if (str.indexOf("$strurl") != -1) {
            QString strurl="";
            strurl.clear();
            strurl.append(Mdev_info.server_addr);
            strurl.append(PING_URL);
            strurl.append("\n");
            QByteArray datasend =strurl.toLocal8Bit();
            write(datasend);
        }
        /*else {
            QString temp = "success\n";
            qDebug()<<temp;
            QByteArray datasend =temp.toLocal8Bit();
            write(datasend);
        }*/
        emit updateClients(str,this->socketDescriptor());
    }
}

void TcpClientSocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}
