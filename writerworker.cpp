#include "writerworker.h"

WriterWorker::WriterWorker(QObject *parent) :
    QThread(parent)
{
    fromAddr = new InetSockAddr();
    stop = false;
}

WriterWorker::WriterWorker(InetSockAddr &serverAddr, InetSockAddr &clientAddr,
                           QString &fileName, QString & mode)
{
    this->localAddr = new InetSockAddr(serverAddr);
    localAddr->setPort(0);

    this->dstAddr = new InetSockAddr(clientAddr);

    try{
        sockCliente = new SocketUDP();
        sockCliente->bind(*localAddr);
    }catch(int e){
        qDebug() << "SenderWorker::Error starting socket.";
    }

    this->fileName = fileName;
    this->openMode = mode;

    this->monitor = new SocketMonitor();    // Agregando socket al monitor.
    monitor->set(*sockCliente, READ);

    this->fromAddr = new InetSockAddr();   // Dirección del remitente.

    stop = false;
}

void WriterWorker::run()
{
    if(openMode == "octet"){
        file.open(fileName.toLocal8Bit().data(), std::ios_base::binary | std::ios_base::out);
    }else if(openMode == "netascii"){
        file.open(fileName.toLocal8Bit().data());
    }

    if(file.is_open()){
        qDebug() << "Opened: " << fileName
                << " in: " << openMode << "mode.";
        if(writeMod == CLIENT){
            timeout.tv_sec=6;
            timeout.tv_usec=0;
            TFTPCommon::sendRRQ(*sockCliente, fileName.toLocal8Bit().data(), OCTET, *dstAddr);
            monitor->select(&timeout);
            if(monitor->isSet(*sockCliente, READ)){
                qDebug() << "Socket is set...";
                int data_size = sockCliente->recvFrom(buff_in, 600, *dstAddr);  // Overwrites 69 port and ip.

                switch(ntohs(*(uint16_t*)(buff_in))){
    //            if(buff_in[0] == 0 && buff_in[1] == 3){     // DATA?
                case 3: //DATA?
                    qDebug() << "DATA recieved, checking block num...";
                    if(1 == ntohs(*((uint16_t*)(buff_in+2)))){
                        qDebug() << "Block matches # " << ntohs(*((uint16_t*)(buff_in+2)));

                        file.write((char*)(buff_in+4), (data_size-4));
                    }else{
                        qDebug() << "Block does not match, recv # " << ntohs(*((uint16_t*)(buff_in+2)));
                    }
                    break;
                case 5:
                    qDebug() << "Error recieved: " << QString(QLatin1String((char*)(buff_in+4)));
                    freeResources();
                    return;
                    break;
                }
            }else{
                qDebug() <<"No response for request...";
                freeResources();
                return;
            }
        }
        getFile();
    }else{
        qDebug() << "Could not open " << fileName <<  "for writting";
        freeResources();
        return;
    }
    emit clienteAtendido(WRQ);
    return;
}

void WriterWorker::setMode(writerMonede mode)
{
    this->writeMod = mode;
}

void WriterWorker::requestStop()
{
    this->stop = true;
}

void WriterWorker::getFile()
{
    int cBlockNum = (writeMod==SERVER)?0:1,
            data_size, wait=0;
    bool last_recv = false, error=false;

    qDebug() << "Recibiendo paquetes... ";

    while(1){
        if(stop){
            qDebug() << "Stopping transfer";
            break;
        }
        timeout.tv_sec=3;
        timeout.tv_usec=0;

        if(error){
            break;
        }
        //if(wait == 0){
            qDebug() << "Sending ACK for block #:" << cBlockNum;

            TFTPCommon::sendACK(*sockCliente, cBlockNum, *dstAddr);
            if(last_recv){
                qDebug() << "Archivo " << fileName
                         << "recibido!.";
                break;
            }
        //}

        try{
            qDebug() << "Waiting for DATA block #" << cBlockNum+1;
            monitor->select(&timeout);
        }catch(int e){
            qDebug() << "Error waiting for ACK...";
            return;
        }

        if(monitor->isSet(*sockCliente, READ)){
            data_size = sockCliente->recvFrom(buff_in, 600, *fromAddr);
            if(data_size < 516){
                last_recv = true;
            }
            if(!(*fromAddr == *dstAddr)){
                qDebug() << "WriterWorkerx =>Source and dst address does not match."
                         << fromAddr->toQString() << "\n" << dstAddr->toQString();
                break;
            }
            switch(ntohs(*(uint16_t*)(buff_in))){
//            if(buff_in[0] == 0 && buff_in[1] == 3){     // DATA?
            case 3: //DATA?
                qDebug() << "DATA recieved, checking block num...";
                if((cBlockNum +1) == ntohs(*((uint16_t*)(buff_in+2)))){
                    qDebug() << "Block matches # " << ntohs(*((uint16_t*)(buff_in+2)));

                    file.write((char*)(buff_in+4), (data_size-4));
                    cBlockNum += 1;
                    wait = 0;
                    continue;
                }else{
                    qDebug() << "Block does not match, recv # " << ntohs(*((uint16_t*)(buff_in+2)));
                }
                break;
            case 5:
                qDebug() << "Error recieved: " << QString(QLatin1String((char*)(buff_in+4)));
                error = true;
                break;
            }
        }else{
            wait += 1;
            qDebug() << "=> Retransmisión: " << wait << " de ACK:" << cBlockNum;
        }
        if(wait > 10){
            qDebug() << "x=> WriteWorker: Número máximo de timeout alcanzado";
            break;
        }
    }
    freeResources();
}

void WriterWorker::freeResources()
{
    sockCliente->close();
    monitor->zeroSet(READ);
    file.close();
}

