#include "senderworker.h"

SenderWorker::SenderWorker(QObject *parent) :
    QThread(parent)
{
}

SenderWorker::SenderWorker(InetSockAddr & serverAddr,InetSockAddr& clientAddr,
                           QString& fileName, QString& mode)
{
    this->localAddr = new InetSockAddr(serverAddr);
    localAddr->setPort(0);  // Se hará bind a un puerto aleatorio.

    this->dstAddr = new InetSockAddr(clientAddr);

    try{
        socketCliente = new SocketUDP();
        socketCliente->bind(*localAddr);
    }catch(int e){
        qDebug() << "SenderWorker::Error starting socket.";
    }

    this->fileName = fileName;
    this->openMode = mode;

    this->monitor = new SocketMonitor();    // Agregando socket al monitor.
    monitor->set(*socketCliente, READ);

    this->remintentAddr = new InetSockAddr();   // Dirección del remitente.

    stop = false;
}

void SenderWorker::run()
{
    if(wMode == CLIENT){
        timeout.tv_sec=3;
        timeout.tv_usec=0;
        TFTPCommon::sendWRQ(*socketCliente, fileName.toLocal8Bit().data(),
                            OCTET, *dstAddr);
        monitor->select(&timeout);
        if(monitor->isSet(*socketCliente, READ)){
            socketCliente->recvFrom(buff_in, 600, *dstAddr);
            qDebug() << "new Server dst: " << dstAddr->toQString();
            putFile();
        }else{
            qDebug() <<"No response for request...";
            socketCliente->close();
            monitor->zeroSet(READ);
            return;
        }
    }else{
        putFile();
    }
    emit clienteAtendido(RRQ);
    return;
}

void SenderWorker::setWriteMode(writerMonede mode)
{
    this->wMode = mode;
}

void SenderWorker::requesStop()
{
    this->stop = true;
}

void SenderWorker::putFile()
{
    int cBlockNum = 1, data_size = -1, data_sent, retransmit = 0;
    timeval timeout;
    bool last_sent = false;

    std::fstream file;

    if(openMode == "octet"){
        file.open(fileName.toLocal8Bit().data(), std::ios_base::binary | std::ios_base::in);
    }else if(openMode == "netascii"){
        file.open(fileName.toLocal8Bit().data());
    }
    qDebug() << "Opened: " << fileName
             << " in: " << openMode << "mode.";

    memset(buff_out, 0, 512);

    while(1){
        if(stop){
            qDebug() << "Stopping transfer";
            break;
        }
        timeout.tv_sec=3;
        timeout.tv_usec=0;
        if(retransmit == 0){
            file.read((char*)buff_out,512);
            data_size = file.gcount();
            qDebug() << data_size << "bytes read from file.";
            if(data_size < 512){
                data_size -= (data_size==0)?0:1;
                last_sent = true;
            }
        }
        // Send block.
        try{
            data_sent = TFTPCommon::sendData(*socketCliente, cBlockNum, buff_out,
                                             data_size, *dstAddr);

            qDebug() << data_sent << "bytes sent to " << dstAddr->toQString();

        }catch(int e){
            qDebug() << "Error sending block " << cBlockNum
                     << "to: " << dstAddr->toQString();
            break;
        }

        // Listen for ACK.
        try{
            qDebug() << "Waiting for ACK #" << cBlockNum;
            monitor->select(&timeout);
        }catch(int e){
            qDebug() << "Error waiting for ACK...";
            break;
        }

        if(monitor->isSet(*socketCliente,READ)){
            qDebug() << "==> sockCliente set for ACK";
            socketCliente->recvFrom(buff_in, 600, *remintentAddr);
            if(!(*remintentAddr == *dstAddr)){  // Verifica que sea el mismo cliente.
                // send error.
                qDebug() << "Source and dst address does not match."
                         << remintentAddr->toQString() << "\n" << dstAddr->toQString();
                break;
            }
            if(buff_in[0] == 0 && buff_in[1] == 4){     // ACK?
                qDebug() << "ACK recieved, checking block num...";
                if(cBlockNum == ntohs(*((uint16_t*)(buff_in+2)))){
                    qDebug() << "Block matches.." << ntohs(*((uint16_t*)(buff_in+2)));
                    cBlockNum += 1;
                    retransmit = 0;
                    if(last_sent){
                        qDebug() << "Archivo " << fileName
                                 << "enviado!.";
                        break;
                    }
                    continue;
                }
            }
        }else{
            retransmit += 1;
            qDebug() << "retransmitiendo bloque:" << cBlockNum;
        }
        if(retransmit > 6){
            qDebug() << "Número máximo de retransmisiones alcanzado";
            break;
        }
        // Check if sender does not ACK,
    }
    socketCliente->close();
    monitor->zeroSet(READ);
    file.close();
}
