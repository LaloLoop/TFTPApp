#ifndef TFTPCLIENT_H
#define TFTPCLIENT_H

#include <QString>

#include "tftpcommon.h"
#include "socketudp.h"
#include "inetsockaddr.h"
#include "socketmonitor.h"
#include "senderworker.h"
#include "writerworker.h"

class TFTPClient: public TFTPCommon{

//    SocketUDP * sLocal;
    InetSockAddr * localAddr, *dstAddr;
//    SocketMonitor * monitor;

    SenderWorker *sWorker;
    WriterWorker *wWorker;

    QString fileName;
    QString octet;

    public:
        TFTPClient(QObject *parent = 0);
        TFTPClient(QString dstIP, qint16 puerto, QString fileName, QObject *parent = 0);
        void setReqInfo(QString dstIP, qint16 puerto, QString fileName);

        void putFile();
        void getFile();

        void stopTransfer();

    public slots:
        virtual void atenderCliente(rqCode, QString, qint16,
                                    QString fileName, QString mode);
        virtual void clienteAtendido(rqCode);
};

#endif // TFTPCLIENT_H
