#ifndef TFTPAPP_H
#define TFTPAPP_H

#include <QMainWindow>
#include <QMessageBox>
#include <QThread>

#include "tftpserver.h"
#include "tftpclient.h"

namespace Ui {
class TFTPApp;
}

class TFTPApp : public QMainWindow
{
        Q_OBJECT

        TFTPServer * server;
        TFTPClient * client;
        
    public:
        explicit TFTPApp(QWidget *parent = 0);
        ~TFTPApp();
        
    private slots:
        void on_pbStartStopServer_pressed();    // Iniciar / Detener el servidor.

        void on_pbPut_pressed();

        void on_pbGet_pressed();

        void on_pbStopClient_pressed();

    private:
        Ui::TFTPApp *ui;
        void toggleClientButtons(bool);

    public slots:
        void workerDone();
};

#endif // TFTPAPP_H
