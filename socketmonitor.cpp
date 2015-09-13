#include "socketmonitor.h"

SocketMonitor::SocketMonitor()
{
    max_fd = -1;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);
    this->utimeout = NULL;
}

void SocketMonitor::clearFrom(Socket & s, fdSet set)
{
    int fd = s.descriptor();
    switch(set){
    case READ:
        FD_CLR(fd, &readfds);
        readSet.erase(&s);
        break;
    case WRITE:
        FD_CLR(fd, &writefds);
        writeSet.erase(&s);
        break;
    case EXCEPTION:
        FD_CLR(fd, &exceptfds);
        exceptSet.erase(&s);
        break;
    }
    if(fd == max_fd){
        getMaxFd();
    }
    qDebug() << "Socket: " << fd << "removed from: " << set << "\n Max fd: " << max_fd;
}

bool SocketMonitor::isSet(Socket & s, fdSet set)
{
    int fd = s.descriptor();
    switch(set){
    case READ:
        if(FD_ISSET(fd, &readfds)){
            return true;
        }
        break;
    case WRITE:
        if(FD_ISSET(fd, &writefds)){
            return true;
        }
        break;
    case EXCEPTION:
        if(FD_ISSET(fd, &exceptfds)){
            return true;
        }
        break;
    }
    return false;
}

void SocketMonitor::set(Socket & s, fdSet set)
{
    int fd = s.descriptor();
    switch(set){
    case READ:
        FD_SET(fd, &readfds);
        readSet.insert(&s);
        break;
    case WRITE:
        FD_SET(fd, &writefds);
        writeSet.insert(&s);
        break;
    case EXCEPTION:
        FD_SET(fd, &exceptfds);
        exceptSet.insert(&s);
        break;
    }
    if(max_fd < fd){
        max_fd = fd;
    }

    qDebug() << "Socket: " << fd << "added to: " << set << "\n Max fd: " << max_fd;
}

void SocketMonitor::zeroSet(fdSet set)
{
    switch(set){
    case READ:
        FD_ZERO(&readfds);
        readSet.empty();
        break;
    case WRITE:
        FD_ZERO(&writefds);
        writeSet.empty();
        break;
    case EXCEPTION:
        FD_ZERO(&exceptfds);
        exceptSet.empty();
        break;
    }
}

// TODO
//int SocketMonitor::select(sockset &readSocks, sockset &writeSocks, sockset &excepSocks, timeval *timeout)
//{
//    readSocks.empty();
//}

int SocketMonitor::select(timeval *timeout)
{
    fd_set * read = NULL, * write = NULL, * except = NULL;

    init_sets();        // Inicializa los conjuntos de descriptores de nuevo.

    if(timeout != NULL){
        this->utimeout = timeout;
    }

    if(readSet.size() != 0){
        read = &readfds;
    }
    if(writeSet.size() != 0){
        write = &writefds;
    }
    if(exceptSet.size() != 0){
        except = &exceptfds;
    }

    int code = ::select(max_fd+1, read, write, except, utimeout);

    if(code < 0){
        qDebug() << "Error[" << errno << "]: " << strerror(errno);
        throw errno;
    }
    return code;
}

void SocketMonitor::setTimeout(timeval * timeout)
{
    this->utimeout = timeout;
}

void SocketMonitor::getMaxFd()
{
    max_fd = -1;
    int fd;
    for(it = readSet.begin(); it != readSet.end(); it++){
        fd = (*it)->descriptor();
        if(fd > max_fd){
            max_fd = fd;
        }
    }
    for(it = writeSet.begin() ; it != writeSet.end() ; it++){
        fd = (*it)->descriptor();
        if(fd > max_fd){
            max_fd = fd;
        }
    }

    for(it = exceptSet.begin() ; it != exceptSet.end() ; it++){
        fd = (*it)->descriptor();
        if(fd > max_fd){
            max_fd = fd;
        }
    }
}

void SocketMonitor::init_sets()
{
    qDebug() << "Inicializando conjuntos para select().";
    for(it = readSet.begin() ; it != readSet.end() ;  it++){
        FD_SET((*it)-> descriptor(), &readfds);
    }
    for(it = writeSet.begin() ; it != writeSet.end() ;  it++){
        FD_SET((*it)-> descriptor(), &writefds);
    }
    for(it = exceptSet.begin() ; it != exceptSet.end() ;  it++){
        FD_SET((*it)-> descriptor(), &exceptfds);
    }
}

