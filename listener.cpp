#include "listener.h"

Listener::Listener(QObject *parent) :
    QThread(parent)
{
    stop = false;
    atendido = true;
}

Listener::Listener(SocketUDP *sockServer /*InetSockAddr & addr*/)
{
    this->sockServer = sockServer;
//    this->serverAddr = new InetSockAddr(addr);
    stop = false;
    atendido = true;
}

Listener::~Listener()
{
    monitor->zeroSet(READ);
    sockServer->close();

    delete monitor;
//    delete serverAddr;
    delete sockServer;
}

void Listener::requestStop()
{
    stop = true;
    qDebug() << "Listener::Stop requested: " << stop;
}

void Listener::run()
{
//    try{
//        sockServer = new SocketUDP();
//    }catch(int e){
//        qDebug() << "Listener::Error creating listener";
//        throw e;
//        return;
//    }
//    try{
//        sockServer->bind(*serverAddr);
//    }catch(int e){
//        qDebug() << "Listener::Error binding server socket";
//        throw e;
//        return;
//    }
    monitor = new SocketMonitor();
    monitor->set(*sockServer, READ);

    qDebug() << "Listener::Executing listener...";
    while(1){
        if(stop){
            qDebug() << "Listener::=>Stopping Listener...";
            sockServer->close();
            sockServer = NULL;
            return;
        }
        try{
            qDebug() << "Listening...";
            monitor->select();
        }catch (int e){
            qDebug() << "Listener:: Error: select()";
            return;
        }
        if(monitor->isSet(*sockServer, READ)){
            clientAddr = new InetSockAddr();

            qDebug() << "Listener::=>Atendiendo cliente";

            memset(buff_in, 0, 600);    // Inicializa el buffer de lectura
                                        // para los mensajes
                                        // en el socket del servidor.
            try{
                sockServer->recvFrom(buff_in, 600, *clientAddr);
            }catch(int e){
                qDebug() << "Listener:: Error reciveing msg from client.";
                return;
            }
            if(buff_in[0] != 0 || buff_in[1] > 5){     // Se verifica el código
                                                       // de operación.
                qDebug() << "Código desconocido";
                return;
            }else{
                decode_request();      // Identifica el tipo de petición y la resuelve.
            }

            while(!atendido){
                msleep(100);
            }
        }
    }
}

void Listener::setAtendido(bool at)
{
    this->atendido = at;
}

void Listener::decode_request(){
    rqCode code;
    QString fileName;
    QString openMode;

    if(buff_in[0] == 0){
        if( buff_in[1] == 1){  // RRQ.

            qDebug() << "=>Petición de lectura.";
            code = RRQ;

        }else if(buff_in[1] == 2){  //WRQ
//            serveWriteRquest();
            qDebug() << "Petición de escritura.desde: " << clientAddr->toQString();
            code = WRQ;
            // Verificación de errores.
        }

        getReqInfo(fileName, openMode); // Obtiene el nombre del archivo y el modo
                                        // de apertura para una petición de lectura.

        qDebug() << "emitMode: "<< fileName << openMode;

        emit nuevaPeticion(code, clientAddr->getIp(),
                           clientAddr->getPort(), fileName,
                           openMode);     // Inicia el proceso de la petición.
        atendido = false;
    }
}

void Listener::getReqInfo(QString& fileName, QString& openMode)
{
    char name[100], mode[20];
    int name_len;
    strcpy(name, (char*)(buff_in + 2));
    name_len = strlen(name);

    fileName = QString(name);
    qDebug() << "requested file: " << fileName << "len:" << name_len;

    strcpy(mode, (char*)(buff_in + 2 + name_len + 1));

    openMode = QString(mode);

    qDebug() << "mode:" << openMode;
}


void Listener::clienteAtendido(bool atend)
{
    this->atendido = atend;
}
