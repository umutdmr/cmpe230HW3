#include "mainwindow.h"

#include <QApplication>

//exec code
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
