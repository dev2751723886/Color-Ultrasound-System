#include "mainwindow.h"
#include <QApplication>
#include<QProcess>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!createMySqlConn()){
        QProcess process;
        process.start("C:/Program Files/MySQL/MySQL Server 8.0/bin/mysqld.exe");
        if(!createMySqlConn()) return 1;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
