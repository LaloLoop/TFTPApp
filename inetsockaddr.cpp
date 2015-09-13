#include "inetsockaddr.h"

InetSockAddr::InetSockAddr()
{
    ini_addr();
}

InetSockAddr::InetSockAddr(uint32_t *ip, uint16_t port)
{
    ini_addr();
    // Llena la estructura.
    ((struct sockaddr_in*)(this->sock_address))->sin_addr.s_addr = htonl(*ip);
    ((struct sockaddr_in*)(this->sock_address))->sin_port = htons(port);
}

//InetSockAddr::InetSockAddr(QString & ip, qint16 port)
//{
//    ini_addr();
//    ipValue(ip,&(((struct sockaddr_in*)(this->sock_address))->sin_addr));
//    ((struct sockaddr_in*)(this->sock_address))->sin_port = htons(port);
//}

InetSockAddr::InetSockAddr(QString ip, qint16 port)
{
    ini_addr();
    ipValue(ip,&(((struct sockaddr_in*)(this->sock_address))->sin_addr));
    ((struct sockaddr_in*)(this->sock_address))->sin_port = htons(port);
}

InetSockAddr::InetSockAddr(InetSockAddr &addr)
{
    ini_addr();
    memcpy((this->sock_address), addr.getRawAddress(), sizeof(struct sockaddr));
}

void InetSockAddr::setIP(QString & ip)
{
    ipValue(ip, &(((struct sockaddr_in*)(this->sock_address))->sin_addr));
}

void InetSockAddr::setPort(qint16 port)
{
    ((struct sockaddr_in*)(this->sock_address))->sin_port = htons(port);
}

qint16 InetSockAddr::getPort()
{
    return ntohs(((struct sockaddr_in*)(this->sock_address))->sin_port);
}

sockaddr *InetSockAddr::getRawAddress()
{
    return this->sock_address;
}

QString InetSockAddr::toQString() const
{
    char *ipString;
    ipString = inet_ntoa(((struct sockaddr_in*)(this->sock_address))->sin_addr);
    uint16_t port = ntohs(((struct sockaddr_in*)(this->sock_address))->sin_port);

    QString legible = (QString(QLatin1String(ipString)) + ":" + QString::number(port));

    return legible;
}

bool InetSockAddr::operator ==(InetSockAddr & addr) const
{
    qDebug() << "Comparing memory struct address";
    qDebug() << "Dst: " << this->toQString() << " Src: " << addr.toQString();
    if(this == &addr){
        return true;
    }
//    else if(!memcmp(this->sock_address, addr.getRawAddress(), sizeof(struct sockaddr))){
//        return true;
//    }
    else if(this->toQString().compare(addr.toQString())){
        return true;
    }
    return true;
}

void InetSockAddr::ini_addr()
{
    // Inicializa a cero.
    addrsize = sizeof(struct sockaddr_in);
    this->sock_address= (struct sockaddr *)calloc(1, addrsize);
    ((struct sockaddr_in*)(this->sock_address))->sin_family = AF_INET;
}

void InetSockAddr::ipValue(QString ip, in_addr * val)
{
    char * ipVal = ip.toLocal8Bit().data();
    if(inet_aton(ipVal, val) == 0){
        qDebug() << "Error[" << errno << "]: " << strerror(errno);
        throw errno;
    }
}

QString InetSockAddr::getIp()
{
    char *ipString;
    ipString = inet_ntoa(((struct sockaddr_in*)(this->sock_address))->sin_addr);
    QString legible = (QString(QLatin1String(ipString)));
    return legible;
}

