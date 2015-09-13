#ifndef LISTENER_H
#define LISTENER_H

#include <QThread>
#include <QString>

#include "socketmonitor.h"
#include "socketudp.h"
#include "inetsockaddr.h"
#include "tftpcommon.h"

class Listener : public QThread
{
        Q_OBJECT

        SocketUDP * sockServer;
//        InetSockAddr  * serverAddr;
        InetSockAddr * clientAddr;

        SocketMonitor * monitor;

        unsigned char buff_in[600];

        volatile bool stop;

        volatile bool atendido;

    public:
        explicit Listener(QObject *parent = 0);
        Listener(/*InetSockAddr&*/ SocketUDP*);

        ~Listener();

        void requestStop();

        void run();

        void setAtendido(bool);
    private:
        void decode_request();
        void getReqInfo(QString&, QString &);

    signals:
        void nuevaPeticion(rqCode tipo, QString ip, qint16 port,
                           QString fileName, QString mode);
    public slots:
        void clienteAtendido(bool);
        
};

#endif // LISTENER_H
