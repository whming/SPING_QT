#include "http_manager.h"
#include <QThread>
const int nHTTP_TIME = 10000; //10秒

Http_Manager::Http_Manager(QObject *parent) :
    QObject(parent)
{
    m_pNetworkManager = NULL;
    m_pNetworkReply = NULL;
//    m_pTimer = new QTimer;
//    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(slot_requestTimeout()));//超时信号
}

Http_Manager::~Http_Manager(){
    if(NULL != m_pNetworkManager){
      delete m_pNetworkManager;
      m_pNetworkManager = NULL;
    }
    if(NULL != m_pTimer){
      delete m_pTimer;
      m_pTimer = NULL;
    }
}

void Http_Manager::GET_Request(const QString& strUrl){
    m_strUrl = strUrl;
    QNetworkRequest netRequest;
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    netRequest.setUrl(QUrl(strUrl)); //地址信息
    if(strUrl.toLower().startsWith("https"))//https请求，需ssl支持(下载openssl拷贝libeay32.dll和ssleay32.dll文件至Qt bin目录或程序运行目录)
    {
        QSslConfiguration sslConfig;
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        sslConfig.setProtocol(QSsl::TlsV1_1);
        netRequest.setSslConfiguration(sslConfig);
    }
    m_pNetworkReply = m_pNetworkManager->get(netRequest); //发起get请求
    connect(m_pNetworkReply,SIGNAL(finished()),this,SLOT(slot_requestTimeout())); //请求完成信号
    m_pTimer->start(nHTTP_TIME);
}

void Http_Manager::Post_Request(const QString& strUrl,const QString& strBody){
    m_strUrl = strUrl;
    QNetworkRequest netRequest;
    QEventLoop loop;
    m_pNetworkManager =  new QNetworkAccessManager;
    m_pTimer = new QTimer;
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    netRequest.setUrl(QUrl(strUrl)); //地址信息
    if(strUrl.toLower().startsWith("https"))//https请求，需ssl支持(下载openssl拷贝libeay32.dll和ssleay32.dll文件至Qt bin目录或程序运行目录)
    {
        QSslConfiguration sslConfig;
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        sslConfig.setProtocol(QSsl::TlsV1_1);
        netRequest.setSslConfiguration(sslConfig);
    }
    QByteArray contentByteArray = strBody.toLatin1();//转成二进制
    m_pNetworkReply = m_pNetworkManager->post(netRequest,contentByteArray);//发起post请求
//    connect(m_pTimer,SIGNAL(timeout()), &loop, SLOT(quit()));//超时信号
//    m_pTimer->start(nHTTP_TIME);
    // 使用事件循环使得网络通讯同步进行
    connect(m_pNetworkReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
    slot_requestFinished();
    m_strUrl.clear();
}

void Http_Manager::slot_requestFinished(){

    if(NULL!=m_pTimer){
          m_pTimer->stop();//关闭定时器
          delete   m_pTimer;
          m_pTimer = NULL;
    }
    QByteArray resultContent = m_pNetworkReply->readAll();
    QTextCodec* pCodec = QTextCodec::codecForName("UTF-8");
    QString strResult = pCodec->toUnicode(resultContent);
    int nHttpCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    if(nHttpCode == 200)//成功
    {
        emit signal_requestFinished(true,strResult);//请求成功
    }
    else
    {
        emit signal_requestFinished(false,strResult);//请求失败
    }
    m_pNetworkReply->deleteLater();
    if(NULL != m_pNetworkManager){
      delete m_pNetworkManager;
      m_pNetworkManager = NULL;
    }
}

void Http_Manager::slot_requestTimeout(){
    //qDebug()<<"slot_requestTimeout strUrl";
    emit signal_requestFinished(false,"timeout");//请求失败
    m_pNetworkReply->deleteLater();
    this->deleteLater();//释放内存
}
