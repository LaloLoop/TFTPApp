#include"tftpclient.h"

TFTPClient::TFTPClient(QObject * parent):
    TFTPCommon(parent)
{
    this->localAddr = new InetSockAddr(QString("0.0.0.0"), 0);
    this->dstAddr = NULL;
    this->sWorker = NULL;
    this->wWorker = NULL;
    octet = "octet";
}

TFTPClient::TFTPClient(QString dstIP, qint16 puerto, QString fileName, QObject *parent):
    TFTPCommon(parent)
{
    this->localAddr = new InetSockAddr(QString("0.0.0.0"), 0);
    this->dstAddr = new InetSockAddr(dstIP, puerto);
    this->fileName = fileName;
    octet = "octet";
}

void TFTPClient::putFile()
{
    sWorker = new SenderWorker(*localAddr, *dstAddr, fileName, octet);
    sWorker->setWriteMode(CLIENT);
    sWorker->start();
}

void TFTPClient::getFile()
{
    wWorker = new WriterWorker(*localAddr, *dstAddr, fileName, octet);
    wWorker->setMode(CLIENT);
    wWorker->start();
}

void TFTPClient::stopTransfer()
{
    if(sWorker != NULL){
        sWorker->requesStop();
    }
    if(wWorker != NULL){
        wWorker->requestStop();
    }
}

void TFTPClient::atenderCliente(rqCode, QString, qint16, QString, QString)
{
}

void TFTPClient::clienteAtendido(rqCode)
{
}

void TFTPClient::setReqInfo(QString dstIP, qint16 puerto, QString fileName)
{
    this->dstAddr = new InetSockAddr(dstIP, puerto);
    this->fileName = fileName;
}

