/* Monitor de datos en el socket. Equivalente a select() y las macros
   relacionadas.
*/
#ifndef SOCKETMONITOR_H
#define SOCKETMONITOR_H

#include <QDebug>

#include <set>

#include <time.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>

#include "socket.h"

typedef std::set<Socket *, SockComp> sockset;   // Definiendo el tipo de conjunto de sockets.

enum fdSet{READ, WRITE, EXCEPTION};             // Conjuntos a los que se pueden agregar sockets

class SocketMonitor
{
    private:
        int max_fd;                 // Valor más alto de alguno de los descriptores de archivo.

        fd_set readfds;             // Conjunto de descriptores a leer.
        fd_set writefds;            // Conjunto de descriptores a escribir.
        fd_set exceptfds;           // Conjunto de descriptores a de excepción.

        struct timeval * utimeout;  // Estructura para timeout (select()).

        // Instancias para la gestión a más alto nivel.
        sockset readSet;            // Conjunto de sockets para lectura.
        sockset writeSet;           // Conjunto de sockets para escritura.
        sockset exceptSet;          // Conjunto de sockets exepción. (errores).
        sockset::const_iterator it; // Iterador sobre los conjuntos de sockets.

    public:
        SocketMonitor();
        void clearFrom(Socket & s, fdSet set);  // Remueve el socket especificado del conjunto dado.
        bool isSet(Socket & s, fdSet set);      // Verifica si un socket ha tenido cambios.
        void set(Socket & s, fdSet set);        // Asigna un socket al conjunto.
        void zeroSet(fdSet set);                // Inicializa el conjunto a cero. El constructor por
                                                // defecto inicializa todos los conjuntos.

//        int select(sockset & readSocks,             // Llena el los conjuntos de sockets, con los
//                   sockset & writeSocks,            // sockets disponibles para las respectivas
//                   sockset & excepSocks,            // operaciones.
//                   struct timeval * timeout = NULL);

        int select(struct timeval * timeout = NULL);    // Hace la operación de select sobre los descriptores
                                                        // que previamente se añadieron con set().

        void setTimeout(struct timeval * timeout);      // Asigna el timeout para la operación select.

    private:
        void getMaxFd();    // Actualiza max_fd, tras haberse elminiado el socket
                            // de valor más alto.

        void init_sets();   // Inicializa los conjuntos de descriptores tras cada
                            // llamada a select().
};

#endif // SOCKETMONITOR_H
