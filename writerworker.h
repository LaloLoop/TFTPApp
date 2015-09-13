#ifndef WRITERWORKER_H
#define WRITERWORKER_H

#include <QThread>
#include "socketudp.h"
#include "socketmonitor.h"
#include "tftpcommon.h"

#include <fstream>
#include <ios>
#include <iostream>

//enum writerMonede{SERVER, CLIENT};

class WriterWorker : public QThread
{
        Q_OBJECT

        SocketUDP * sockCliente;    // Socket escritura/lectura de paquetes.

        SocketMonitor *monitor;     // Monitor de socket.

        InetSockAddr * localAddr,   // Dirección de socketCliente.
                     *fromAddr,     // Dirección de los paquetes recibidos.
                     *dstAddr;      // Dirección de destino.

        unsigned char buff_in[600];
        unsigned char buff_out[512];

        QString fileName;
        QString openMode;

        writerMonede writeMod;

        std::fstream file;

        struct timeval timeout;

        volatile bool stop;

    public:
        explicit WriterWorker(QObject *parent = 0);
        WriterWorker(InetSockAddr & serverAddr, InetSockAddr & clientAddr,
                     QString &fileName, QString&);
        void run();

        void setMode(writerMonede);     // Especifica la operación a realizar.

        void requestStop();

    private:
        void getFile();     // Atiende la petición de escritura
                            // en el servidor.
        void freeResources();
    signals:
        void clienteAtendido(rqCode);

    public slots:

};

#endif // WRITERWORKER_h
