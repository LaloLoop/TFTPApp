#ifndef INETADDR_H
#define INETADDR_H

#include <QString>
#include <QStringList>
#include <malloc.h>
#include <QDebug>

class inetAddr
{
protected:
    unsigned char address[4];
public:
    inetAddr();
    inetAddr(QString);
    inetAddr(unsigned char *);
    QString toString(void);
    void get_raw_address(unsigned char copy[4]);
    void set_address(unsigned char addr[4]);
};

#endif // INETADDR_H
