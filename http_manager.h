#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QTimer>
#include <QSslConfiguration>
#include <QString>
#include <QByteArray>
#include <QTextCodec> 
#include <QEventLoop>

class Http_Manager : public QObject
{
    Q_OBJECT

public:
    explicit Http_Manager(QObject *parent = 0);
    ~Http_Manager();
    void GET_Request(const QString& strUrl);//根据url发起http请求
    void Post_Request(const QString& strUrl,const QString& strBody);
signals:
    void signal_requestFinished(bool bSuccess,const QString& strResult); //http请求结束
public slots:
    void slot_requestFinished(); //http请求结束
    void slot_requestTimeout();  //请求超时

private:
    QNetworkAccessManager* m_pNetworkManager;//网络管理类
    QNetworkReply* m_pNetworkReply; //封装请求返回信息
    QTimer* m_pTimer; //请求超时计时器
    QString m_strUrl; //记录当前请求的url

};

#endif // HTTP_MANAGER_H
