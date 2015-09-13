#-------------------------------------------------
#
# Project created by QtCreator 2012-02-21T00:25:02
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = TFTPApp
TEMPLATE = app


SOURCES += main.cpp\
        tftpapp.cpp \
    tftpserver.cpp \
    tftpcommon.cpp \
    tftpclient.cpp \
    socketudp.cpp \
    socketmonitor.cpp \
    socket.cpp \
    sockaddr.cpp \
    inetsockaddr.cpp \
    listener.cpp \
    senderworker.cpp \
    writerworker.cpp

HEADERS  += tftpapp.h \
    tftpserver.h \
    tftpcommon.h \
    tftpclient.h \
    socketudp.h \
    socketmonitor.h \
    socket.h \
    sockaddr.h \
    inetsockaddr.h \
    listener.h \
    senderworker.h \
    writerworker.h

FORMS    += tftpapp.ui
