#ifndef SOCKETUDP_H
#define SOCKETUDP_H

#include <QDebug>

#include <errno.h>
#include <string.h>

#include "socket.h"
#include "inetsockaddr.h"

class SocketUDP : public Socket
{
    public:
//        SocketUDP();
        SocketUDP(int protocol = 0);

        void open(int protocol = 0);

        void bind(SockAddr & addr);                 // Bind a addr.
        void bind(uint32_t * ip, uint16_t port);    // Overload. Bind a ip:puerto.

        virtual int sendTo(const void * message,    // Envía mensaje a la
                    size_t length,                  // dirección especificada
                    SockAddr & dstaddr);            // en dstaddr.

        virtual int recvFrom(void * buffer,   // Recibe mensajes
                     size_t length,                 // en este socket.
                     SockAddr & from);              // from: dirección origen.

        // TODO.
//        virtual void setSockOpt();
//        virtual void getSockOpt();
//        virtual void getSockName();

};

#endif // SOCKETUDP_H
