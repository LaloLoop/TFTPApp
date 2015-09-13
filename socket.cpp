#include "socket.h"

// ------------------------------------ CONSTRUCTORES ------------------------------------//
Socket::Socket()
{
    sock_d = -1;
}

Socket::Socket(int domain, int type, int protocol)
{
    int code = open_sock(domain, type,  protocol);
    if(code != 0){
        throw code;
    }
}

// ------------------------------------ MÉTODOS PÚBLICOS ------------------------------------//
// open: Función para abrir el socket, sólo se debería llamar en caso
// de haber usado el constructor por defecto.
void Socket::open(int domain, int type, int protocol)
{
    if(sock_d > 0){
        qDebug() << "Socket::open: el socket ya ha sido abierto.";
    }else{
        int code = open_sock(domain, type, protocol);
        if(code != 0){
            throw code;
        }
    }
}

int Socket::descriptor()
{
    return this->sock_d;
}

// close: Cierra el socket.
void Socket::close()
{
    if(::close(sock_d) < 0){    // Use "::" for global namespace.
        qDebug() << "Socket::close:Error[" << errno << "]: " << strerror(errno);
        throw errno;            // Could not close socket.
    }
    qDebug() << "close:socket closed.";
}

//// sendData: Envío de información a través del socket.
//void Socket::sendData(unsigned char *buf, size_t buf_len)
//{
//    int data_len = -1;

//    struct sockaddr_in * addr;

//    addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
//    if(addr == NULL){
//        qDebug() << "send_frame::Error[malloc] if_out es nulo.\n" ;
//        throw 40;
//    }

//    /* Definiendo los valores de la dirección IP destino */
//    addr -> sin_family = AF_INET;
//    addr -> sin_port   = htons(3213);
//    inet_aton("192.168.1.254",&(addr->sin_addr));

//    data_len = sendto(sock_d, (const void *)buf, buf_len, 0, (struct sockaddr *)addr, sizeof(struct sockaddr_in));

//    if(data_len < 0){
//        qDebug() << "send_frame::Error[" << errno << "]: " << strerror(errno);
//        free(addr);
//        throw 30;
//      }
//      else{
//        qDebug() << "Data sent: " << QString::fromAscii((char*)buf) << "len: " << buf_len;
//        free(addr);
//    }
//}

// ------------------------------------ MÉTODOS PRIVADOS ------------------------------------//
// open_sock: Método empleado por el construtcor y open();
// Devuelve errno en caso de error o 0 si todo ocurrió normalmente.
int Socket::open_sock(int domain, int type, int protocol)
{
    sock_d = socket(domain, type, protocol);
    if(sock_d < 0){
        qDebug() << "Socket::open_sock::Error[" << errno << "]: " << strerror(errno);
        return errno;
    }
    qDebug() << "socket opened: " << sock_d;
    return 0;
}

