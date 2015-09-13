#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "tftpapp.h"

int main(int argc, char *argv[])
{
    //Coodificación del texto.
    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
//    QTextCodec::setCodecForTr(linuxCodec);
//    QTextCodec::setCodecForCStrings(linuxCodec);
    QTextCodec::setCodecForLocale(linuxCodec);

    QApplication a(argc, argv);
    TFTPApp w;
    w.show();
    
    return a.exec();
}
