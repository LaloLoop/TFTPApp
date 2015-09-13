#include "inetaddr.h"

inetAddr::inetAddr()
{
    memset(address,0,sizeof(address));
}

inetAddr::inetAddr(unsigned char *ip)
{
    memcpy((void*)address,(void*)ip,4);
}

inetAddr::inetAddr(QString ip)
{
    int i;
    QStringList octets = ip.split(".");
    for(i = 0; i < 4 ; i++){
        address[3-i] = octets.at(i).toInt();
    }
//    qDebug() << "::IP:" << address[3] << "." << address[2] << "." << address[1] << "." << address[0];
}

QString inetAddr::toString(){
    return QString::number(address[3]) +
            "." + QString::number(address[2]) +
            "." + QString::number(address[1]) +
            "." + QString::number(address[0]);
}
void inetAddr::get_raw_address(unsigned char copy[4]){
    memcpy(copy,address,4);
    return;
}
void inetAddr::set_address(unsigned char addr[4]){
    memcpy(address,addr,4);
    return;
}
