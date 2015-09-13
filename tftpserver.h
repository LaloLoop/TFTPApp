#ifndef TFTPSERVER_H
#define TFTPSERVER_H

#include <QString>

#include "tftpcommon.h"
#include "socketmonitor.h"
#include "listener.h"
#include "senderworker.h"
#include "writerworker.h"

class TFTPServer : public TFTPCommon{

        SocketUDP * sockServer;
        InetSockAddr * serverAddr;

        QString *currentDir;

        Listener * cListener;   // Detectar nuevos clientes.
        SenderWorker *sWorker;  // Atiende peticiones de lectura.
        WriterWorker *wWorker;  // Atiende peticiones de escritura.

        bool running;

    public:
        explicit TFTPServer(QObject * parent = 0);
        TFTPServer(QString ip, qint16 port, QObject *parent = 0);
        ~TFTPServer();

        void start();        // Inicia el servidor.
        void stop();         // Detiene el servidor.

        bool isRunning();    // Is running?
        void setRunning(bool);

    public slots:
        void atenderCliente(rqCode, QString, qint16, QString fileName, QString mode);      // Atender la petición del cliente.
        void clienteAtendido(rqCode);

};

#endif // TFTPSERVER_H
