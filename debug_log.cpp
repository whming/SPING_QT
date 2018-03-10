#include "debug_log.h"

using namespace std ;

Debug_Log::Debug_Log(int unit)
{
    Num_Unit = unit;
    _logLevel = LOG_DEBUG;
    _logPath = QDir::currentPath();
}

Debug_Log::~Debug_Log()
{

}

void Debug_Log::PrintLog(LOGLEVEL level, const char *msg,  ...)
{
    if (level <_logLevel)  return;     //low level
    char logBuffer[8192] = {0};
    va_list vl_fmt;                     //buffer
    va_start(vl_fmt, msg);
    vsprintf(logBuffer, msg, vl_fmt);
    va_end(vl_fmt);

    QString fileTime = "";
    QString logTime = "";
    QString logLevel = getLevelStr(level);
    fileTime = QDateTime::currentDateTime().toString("yyyyMMdd");
    logTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    qDebug("[%s] [%s] {%s}", logTime.toStdString().c_str(), logLevel.toStdString().c_str(), logBuffer);
}

void Debug_Log::SaveFileLog(LOGLEVEL level, const char *msg,  ...)
{
    if (level < _logLevel)  return;  //low level
    char logBuffer[8192] = {0};
    va_list vl_fmt;                  //buff
    va_start(vl_fmt, msg);
    vsprintf(logBuffer, msg, vl_fmt);
    va_end(vl_fmt);

    QString logTime = "";
    QString fileTime = "";
    fileTime = QDateTime::currentDateTime().toString("yyyyMMdd");
    logTime = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz]");
    QString logLevel = getLevelStr(level);
    QString logFile = _logPath;
    if (logFile.right(1) != "/") {
        logFile += "/";
    }
    QDir mDir(logFile);
    if (!mDir.exists()) {
        mDir.mkpath(logFile);
    }

    logFile += "APN_Unit_";
    logFile +=QString::number(Num_Unit);
    logFile += "_";
    logFile += fileTime;
    logFile += ".log";

    QFile file(logFile);
    file.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    out << logTime << " [" << logLevel << "] " << "{" << logBuffer << "}" << endl;
    file.close();
}

QString Debug_Log::getLevelStr(LOGLEVEL level)
{
    switch(level) {
        case LOG_DEBUG: return "LOG_DEBUG"; break;
        case LOG_INFO: return "LOG_INFO"; break;
        case LOG_WARN: return "LOG_WARN"; break;
        case LOG_ERROR: return "LOG_ERROR"; break;
    }
}

void Debug_Log::setLogPath(QString logPath)
{
    _logPath = logPath;
}

void Debug_Log::setLogLevel(LOGLEVEL logLevel)
{
    _logLevel = logLevel;
}
