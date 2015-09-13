#include "tftpserver.h"

TFTPServer::TFTPServer(QObject *parent):
    TFTPCommon(parent)
{
    this->currentDir = new QString("/Users/Lalo/Documents/Develop/Redes/TFTPTest/");
    this->running = false;
    this->cListener = NULL;
}

TFTPServer::TFTPServer(QString ip, qint16 port, QObject * parent):
    TFTPCommon(parent)
{
    this->currentDir = new QString("/Users/Lalo/Documents/Develop/Redes/TFTPTest/");
    this->serverAddr = new InetSockAddr(ip, port);
    this->running = false;
    this->cListener = NULL;
}

TFTPServer::~TFTPServer()
{
    this->stop();
    delete cListener;
    delete sockServer;
}

void TFTPServer::start()
{
    try{
        sockServer = new SocketUDP();
    }catch(int e){
        qDebug() << "TFTPServer::Error creating listener";
        throw e;
    }
    try{
        sockServer->bind(*serverAddr);
    }catch(int e){
        qDebug() << "TFTPServer::Error binding server socket";
        this->stop();
        throw e;
    }
    qDebug() << "TFTPServer::=>Starting server...";
    try{
        cListener = new Listener(sockServer);
        connect(cListener, SIGNAL(nuevaPeticion(rqCode,QString,qint16,QString,QString)),
                this, SLOT(atenderCliente(rqCode,QString,qint16,QString,QString)));
    }catch(int e){
        qDebug() << "TFTPServer::start(): Error: couldn't allocate Listener.";
        this->stop();
        throw e;
    }

    try{
        cListener->start();
        running = true;
    }catch(int e){
        qDebug() << "TFTPServer::start(): Error: couldn't start Listener.";
        this->stop();
        throw e;
    }
}

void TFTPServer::stop()
{
    qDebug() << "=>Stopping server...";
    running = false;
    if(cListener!=NULL){
        disconnect(cListener,SIGNAL(nuevaPeticion(rqCode,QString,qint16,QString,QString)),
                   this, SLOT(atenderCliente(rqCode,QString,qint16,QString,QString)));
        cListener->requestStop();
    }
    sockServer->close();
    sockServer = NULL;
}

bool TFTPServer::isRunning()
{
    return running;
}

void TFTPServer::setRunning(bool run)
{
    this->running = run;
}

void TFTPServer::atenderCliente(rqCode code, QString ip,
                                qint16 port, QString fileName,
                                QString mode)
{
    InetSockAddr * cAddr = new InetSockAddr(ip,port);
    qDebug() << "TFTPServer::Atender: " << code << " en " << cAddr->toQString();
    switch(code){
    case RRQ:
        try{
            QString finalPath = (*currentDir + fileName);
            sWorker = new SenderWorker(*serverAddr,*cAddr, finalPath, mode);
            connect(sWorker, SIGNAL(clienteAtendido(rqCode)),
                    this, SLOT(clienteAtendido(rqCode)));
            sWorker->setWriteMode(SERVER);
            cListener->setPriority(QThread::LowestPriority);
            sWorker->start();
        }catch(int e){
            qDebug() << "TFTP::Server Error starting Send worker";
        }
        break;
    case WRQ:
        try{
            QString finalPath = (*currentDir + fileName);
            wWorker = new WriterWorker(*serverAddr, *cAddr, finalPath, mode);
            wWorker -> setMode(SERVER);
            connect(wWorker, SIGNAL(clienteAtendido(rqCode)),
                    this, SLOT(clienteAtendido(rqCode)));
            cListener->setPriority(QThread::LowestPriority);
            wWorker->start();
        }catch(int e){
            qDebug() << "TFTP::Server Error starting Writer worker";
        }
        break;
    }
}

void TFTPServer::clienteAtendido(rqCode code)
{
    switch (code){
    case RRQ:
        disconnect(sWorker, SIGNAL(clienteAtendido(rqCode)),
                   this, SLOT(clienteAtendido(rqCode)));
        break;
    case WRQ:
        disconnect(wWorker, SIGNAL(clienteAtendido(rqCode)),
                   this, SLOT(clienteAtendido(rqCode)));
        break;
    }
    cListener->setPriority(QThread::InheritPriority);
    cListener->setAtendido(true);
}

