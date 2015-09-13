/* Socket Internet Address class.
   Maneja direcciones de internet para sockets de la familia AF_INET.
*/
#ifndef INETSOCKADDR_H
#define INETSOCKADDR_H

#include <QString>
#include <QDebug>

// Funciones para IP
#include <netinet/in.h>
#include <netinet/ip.h>
// inet_aton(), ...
#include <arpa/inet.h>
// Errores
#include <errno.h>

#include "sockaddr.h"

class InetSockAddr : public SockAddr
{

    public:
        InetSockAddr();
        InetSockAddr(uint32_t * ip, uint16_t port); // ip: Obtenida mediante inet_aton o similares.
//        InetSockAddr(QString & ip, qint16 port);    // ip en formao 'x.x.x.x'.
        InetSockAddr(QString ip, qint16 port);
        InetSockAddr(InetSockAddr & addr);          // Constructor por copia.

        void setIP(QString & ip);                   // Changes ip address value.
        void setPort(qint16 port);                  // Change address port.

        qint16 getPort();                           // Gets port in host order.
        QString getIp();

        struct sockaddr * getRawAddress();          // Se obtiene la estructura tal cual.

        QString toQString() const;                        // Imprime la información de la dirección.

        bool operator==(InetSockAddr &) const;      // Compara direcciones.

    private:
        void ini_addr();                            // Inicializa la longitud y estructura a cero.
        void ipValue(QString ip, in_addr *val);      // Convierte QString a IP.
};

#endif // INETSOCKADDR_H
