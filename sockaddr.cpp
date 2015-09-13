#include "sockaddr.h"

SockAddr::SockAddr()
{
    ini_addr();
}

int SockAddr::length() const
{
    return this->addrsize;
}

sockaddr *SockAddr::getRawAddress()
{
    return this->sock_address;
}

void SockAddr::ini_addr()
{
    this->addrsize = sizeof(struct sockaddr);
    this->sock_address = (struct sockaddr *)calloc(1, this->addrsize);
}

