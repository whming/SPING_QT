#-------------------------------------------------
#
# Project created by QtCreator 2018-01-02T17:29:23
#
#-------------------------------------------------

QT       += core gui
QT       += widgets axcontainer
QT       += serialport sql
QT       += network concurrent
RC_FILE     += appico.rc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_DEPRECATED_WARNINGS
TARGET = SimPing
TEMPLATE = app


SOURCES += main.cpp\
    main_widget.cpp \
    debug_log.cpp \
    server.cpp \
    tcpclientsocket.cpp \
    base_data.cpp \
    http_manager.cpp \
    sql_api.cpp \
    cJSON.c

HEADERS  += \
    main_widget.h \
    debug_log.h \
    server.h \
    tcpclientsocket.h \
    base_data.h \
    cJSON.h \
    http_manager.h \
    Identification.h \
    sql_api.h

FORMS    += \
    main_widget.ui

RESOURCES += \
    src.qrc

OTHER_FILES += \
    appico.rc
