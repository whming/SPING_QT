#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H
#include <QString>
#include <QtGui>
enum LOGLEVEL{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
};

class Debug_Log
{
public:
    explicit Debug_Log(int unit);
    ~Debug_Log();
//    static Debug_Log* getLogCenter();
    void PrintLog(LOGLEVEL level, const char* msg, ...);
    void SaveFileLog(LOGLEVEL level, const char* msg, ...);
    void setLogPath(QString logPath);         //defalut: current path
    void setLogLevel(LOGLEVEL logLevel);      //defalut: LOG_DEBUG
//private:
//    /*static */Debug_Log * _logCenter;

private:
    QString _logPath;
    LOGLEVEL _logLevel;
    int Num_Unit;
private:
    QString getLevelStr(LOGLEVEL level);
};

#endif // DEBUG_LOG_H
