/* Clase Abstracta.
   Características similares para el cliente y servidor del protocolo TFTP.
*/

#ifndef TFTPCOMMON_H
#define TFTPCOMMON_H

#include <QObject>

#include "socketudp.h"

// Tipos de envío.
#define NETASCII 9
#define OCTET 8
// Errores.
#define FILE_NOT_FOUND 1
#define ACCES_VIOLATION 2
#define NO_MEMORY 3
#define ILLEGAL_TFTP 4
#define UNKNOWN_ID 5
#define FILE_EXISTS 6
#define NO_SUCH_USER 7

typedef int tid_t;  // Identificador de transimisión

enum rqCode {RRQ, WRQ};
enum writerMonede{SERVER, CLIENT};

class TFTPCommon : public QObject{
    Q_OBJECT

//    protected:
//        tid_t tid;

    public:
        TFTPCommon(QObject *parent);
        static int sendACK(SocketUDP sock,unsigned short block,SockAddr &addr);
        static int sendData(SocketUDP sock,unsigned short block,void* data,size_t data_size,SockAddr &addr);
        static int sendError(SocketUDP sock,int errtype,SockAddr &addr);
        static int sendWRQ(SocketUDP sock,const char* filename,int mode,SockAddr &addr);
        static int sendRRQ(SocketUDP sock,const char* filename,int mode,SockAddr &addr);

//    protected:
//        void generateTID(); // Genera un TID aleatorio.
    public slots:
        virtual void atenderCliente(rqCode, QString, qint16,
                                    QString fileName, QString mode) = 0;
        virtual void clienteAtendido(rqCode) = 0;

};

#endif // TFTPCOMMON_H
