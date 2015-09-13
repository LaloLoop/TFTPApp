/* Socket Address: Clase base para las direcciones que
   se pueden asignar a un socket mediante bind().
*/

#ifndef SOCKADDR_H
#define SOCKADDR_H

#include <QString>

#include <sys/socket.h>
#include <stdlib.h>

class SockAddr
{
    protected:
        struct sockaddr * sock_address;             // Dirección de socket (man 2 bind).
        int addrsize;                               // Tamaño de la dirección.

    public:
        SockAddr();
        int length() const;                         // Longitud de este tipo de dirección.
        virtual struct sockaddr * getRawAddress();  // Bytes de la dirección.
        virtual QString toQString() const = 0;      // Dirección en formato legible.

    protected:
        virtual void ini_addr();                    // Inicializa la longitud y estructura a cero.
};

#endif // SOCKADDR_H
