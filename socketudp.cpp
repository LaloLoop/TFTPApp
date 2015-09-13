#include "socketudp.h"

//SocketUDP::SocketUDP()
//{
//    this->domain = AF_INET;
//    this->type = SOCK_DGRAM;
//    sock_d = -1;
//}

SocketUDP::SocketUDP(int protocol):
    Socket(AF_INET, SOCK_DGRAM, protocol)
{
}

void SocketUDP::open(int protocol)
{
    this->domain = AF_INET;
    this->type = SOCK_DGRAM;

    if(sock_d > 0){
        qDebug() << "SocketUDP::open: el socket ya ha sido abierto.";
    }else{
        int code = open_sock(this->domain, this->type, protocol);
        if(code != 0){
            throw code;
        }
    }
}

void SocketUDP::bind(SockAddr & addr)
{
    int code = ::bind(this->sock_d,(addr.getRawAddress()),addr.length());
    if(code < 0){
        qDebug() << "Error [" << errno << "]: " <<  strerror(errno);
        throw errno;
    }
    qDebug() << "bin: socket binded to " << addr.toQString();
}

void SocketUDP::bind(uint32_t *ip, uint16_t port)
{
    InetSockAddr addr(ip, port);
    this->bind(addr);
}

int SocketUDP::sendTo(const void *message, size_t length, SockAddr & dstaddr)
{
    int sent = ::sendto(this->sock_d, message, length, 0, dstaddr.getRawAddress(),dstaddr.length());
    if(sent < 0){
        qDebug() << "Error[" << errno << "]:" << strerror(errno);
        throw errno;
    }
    qDebug() << "sendTo: " << sent << " bytes sent.";
    return sent;
}

int SocketUDP::recvFrom(void *buffer, size_t length, SockAddr &from)
{
    unsigned int addr_length = from.length();
    int read = ::recvfrom(this->sock_d,buffer, length, 0, from.getRawAddress(), &addr_length);
    if(read < 0){
        qDebug() << "Error[" << errno << "]:" << strerror(errno);
        throw errno;
    }
    qDebug() << "recvFrom: " << read << "bytes recieved.";
    return read;
}

//void SocketUDP::setSockOpt()
//{
//}

//void SocketUDP::getSockOpt()
//{
//}

//void SocketUDP::getSockName()
//{
//}

