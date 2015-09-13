/* Server Worker Atiende las peticiones de lectura y escritura. */
#ifndef SENDERWORKER_H
#define SENDERWORKER_H

#include <QThread>

#include <fstream>
#include <iostream>
#include <ios>

#include "socketudp.h"
#include "socketmonitor.h"
#include "inetsockaddr.h"
#include "tftpcommon.h"

class SenderWorker : public QThread
{
        Q_OBJECT

        SocketUDP * socketCliente;

        SocketMonitor * monitor;

        InetSockAddr * localAddr;       // Dirección en la que se esucha.
        InetSockAddr  * remintentAddr;  // Dirección recibida del mensaje.
        InetSockAddr * dstAddr;         // Dirección de envío de los mensajes.

        unsigned char buff_in[600];
        unsigned char buff_out[512];

        QString fileName;
        QString openMode;

        writerMonede wMode;

        volatile bool stop;

        struct timeval timeout;

    public:
        explicit SenderWorker(QObject *parent = 0);
        SenderWorker(InetSockAddr & serverAddr, InetSockAddr& clientAddr,
                     QString &fileName, QString& mode);

        void run();

        void setWriteMode(writerMonede);

        void requesStop();

    signals:
        void clienteAtendido(rqCode);
        
    public slots:

    private:

        void putFile();     // Atiende la petición de lectura.
};

#endif // SENDERWORKER_H
