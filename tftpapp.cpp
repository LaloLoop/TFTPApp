#include "tftpapp.h"
#include "ui_tftpapp.h"

TFTPApp::TFTPApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TFTPApp)
{
    ui->setupUi(this);
    qRegisterMetaType<rqCode>("rqCode");
    server = NULL;
    client = NULL;
}

TFTPApp::~TFTPApp()
{
    delete ui;
//    if(server != NULL){
//        delete server;
//    }
//    if(client != NULL){
//        delete client;
//    }
}

void TFTPApp::on_pbStartStopServer_pressed()
{
    if(server != NULL){
        if(server->isRunning()){
            server->stop();
            server = NULL;
            ui->pbStartStopServer->setText("Iniciar");
        }
    }else{
        server = new TFTPServer(ui->leServerIP->text(),
                                (qint16)ui->spServerPort->value(), this);
        try{
            server->start();
            ui->pbStartStopServer->setText("Detener");
        }catch(int e){
            QMessageBox::information(this, "Error! =(", "Error al iniciar el servidor. \nVea el Log para más información.");
            server = NULL;
        }
    }
}

void TFTPApp::on_pbPut_pressed()
{
//    toggleClientButtons(false);

    if(client == NULL){
        client = new TFTPClient(ui->leIPHost->text(), (qint16)ui->spPort->value(),
                                ui->leFileName->text(),this);
//        connect(client, SIGNAL(workerDone()), this, SLOT(workerDone()));
    }else{
        client->setReqInfo(ui->leIPHost->text(), (qint16)ui->spPort->value(),
                       ui->leFileName->text());
   }
//    connect(client, SIGNAL(workerDone(bool)), this, SLOT(workerDone()));
    client->putFile();
}

void TFTPApp::on_pbGet_pressed()
{
//    toggleClientButtons(false);

    if(client == NULL){
        client = new TFTPClient(ui->leIPHost->text(), (qint16)ui->spPort->value(),
                                ui->leFileName->text(), this);
    }else{
        client->setReqInfo(ui->leIPHost->text(), (qint16)ui->spPort->value(),
                       ui->leFileName->text());
   }
    client->getFile();
}

void TFTPApp::on_pbStopClient_pressed()
{
    if(client != NULL){
        client->stopTransfer();
    }
//    toggleClientButtons(true);
}

void TFTPApp::toggleClientButtons(bool val)
{
    ui->pbGet->setEnabled(val);
    ui->pbPut->setEnabled(val);
    ui->pbStopClient->setEnabled(!val);
}

void TFTPApp::workerDone()
{
    toggleClientButtons(true);
}
