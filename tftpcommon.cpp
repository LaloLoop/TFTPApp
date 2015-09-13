#include "tftpcommon.h"

int TFTPCommon::sendACK(SocketUDP sock, unsigned short block,SockAddr &addr){

    unsigned short* data=new unsigned short[2];

    data[0]=htons(0x0004);
    data[1]=htons(block);

    return sock.sendTo((void*)data,sizeof(short)*2,addr);
}

int TFTPCommon::sendData(SocketUDP sock, unsigned short block, void *data, size_t data_size, SockAddr &addr){

    if(data_size>512)
        data_size=512;

    unsigned char* data_s=new unsigned char[4+data_size];

    data_s[0]=0x00;
    data_s[1]=0x03;
    data_s[2]=(unsigned char)((0x00FF)&(block>>8));
    data_s[3]=(unsigned char)((0x00FF)&(block));

    memcpy(data_s+4,data,data_size);

    return sock.sendTo((void*)data_s,sizeof(char)*4+data_size,addr);
}

int TFTPCommon::sendError(SocketUDP sock, int errtype, SockAddr &addr){

    unsigned char* data_s=new unsigned char[6];

    data_s[0]=0x00;
    data_s[1]=0x05;
    data_s[2]=(unsigned char)((0x000000FF)&(errtype>>8));
    data_s[3]=(unsigned char)((0x000000FF)&(errtype));
    data_s[4]='X';//Pendientes las cadenas de error
    data_s[5]=0x00;


    return sock.sendTo((void*)data_s,sizeof(char)*5,addr);

}

int TFTPCommon::sendRRQ(SocketUDP sock, const char *filename, int mode, SockAddr &addr){

    unsigned char* data_s=new unsigned char[15+strlen(filename)];

    data_s[0]=0x00;
    data_s[1]=0x01;


    strcpy((char*)(data_s+2),filename);

    if(mode==OCTET){
        strcpy((char*)(data_s+3+strlen(filename)),"octet");
    }
    else if(mode==NETASCII){
       strcpy((char*)(data_s+3+strlen(filename)),"netascii");
    }
    else return -1;


    return sock.sendTo((void*)data_s,sizeof(char)*(4+strlen(filename)+(mode==OCTET?5:8)),addr);
}

int TFTPCommon::sendWRQ(SocketUDP sock, const char *filename, int mode, SockAddr &addr){

    unsigned char* data_s=new unsigned char[15+strlen(filename)];

    data_s[0]=0x00;
    data_s[1]=0x02;


    strcpy((char*)(data_s+2),filename);

    if(mode==OCTET){
        strcpy((char*)(data_s+3+strlen(filename)),"octet");
    }
    else if(mode==NETASCII){
       strcpy((char*)(data_s+3+strlen(filename)),"netascii");
    }
    else return -1;


    return sock.sendTo((void*)data_s,sizeof(char)*(4+strlen(filename)+(mode==OCTET?5:8)),addr);
}

TFTPCommon::TFTPCommon(QObject *parent):
    QObject(parent)
{
}
