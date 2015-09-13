/* Socket abstract class.
   Empleada por las clases de sockets más específicos
   como sockets de la familia AF_INET.
   Cada familia de sockets que se añada deberá implementar los métodos
   virtuales puros aquí descritos.
*/

#ifndef SOCKET_H
#define SOCKET_H
#include <QDebug>
#include <QString>

// --- Cabeceras UNIX ---
// Funciones de socket.
#include <sys/socket.h>

// Close.
#include <unistd.h>

// Errores
#include <string.h>
#include <errno.h>

#include "sockaddr.h"

class Socket
{
    protected:
        int sock_d;                                             // Descriptor de archivo del socket.
        int domain;                                             // Dominio del socket.
        int type;                                               // Tipo de socket.
        int protocol;                                           // Protocolo a filtrar.
        int level;                                              // Nivel de la API de sockets.


    public:
        Socket();                                               // Inicializa sock_d a un valor inválido (-1).

        Socket(int domain, int type, int protocol);             // Abre el socket con los parámetros indicados.

        int descriptor();                                       // Regresa el descriptor de archivo.
                                                                // Puede llegar a ser usado en funciones de
                                                                // enviar y recibir trama.

        void close();                                           // Cierra el socket.

        virtual void open(int domain, int type, int protocol);  // Abre el socket sólo si se ha usado
                                                                // el constructor por defecto.

//      virtual Socket accept() = 0;                            // Empleado en SOCK_STREAM y SOCK_SEQPACKET.
                                                                // No se implmenta aquí.

        virtual void bind(SockAddr &) = 0;                      // Aignar a una dirección y puerto locales al socket

//      virtual void listen() = 0;                              // Tells the socket that new connections shall be accepted.
                                                                // Usado con SOCK_STREM y SOCK_SEQPACKET.
                                                                // No se implmenta aquí.

        virtual int sendTo(const void * message,                // Envía mensajes a través del socket.
                           size_t length,
                           SockAddr & dstaddr) = 0;

        virtual int recvFrom(void * buffer,                     // Recibe mensajes a través del socket.
                             size_t length,
                             SockAddr & from) = 0;

//        virtual void setSockOpt(Socket &, int optionName,
//                                void * optionValue, socklen_t len) = 0;                          // Asigna opciones al socket.

//        virtual void getSockOpt() = 0;                          // Obtiene opciones del socket.

//        virtual void getSockName() = 0;                         // Devuelve la dirección a la que se ha hecho bind.

    protected:
        int open_sock(int domain, int type, int protocol);      // Empleada por el constructor y open.
                                                                // En caso de error al abrir el socket lanza excepción.
};

class SockComp{
    public:
        bool operator()(Socket * s1, Socket * s2) const{
            return (s1->descriptor() < s2->descriptor());
        }
};

#endif // SOCKET_H
