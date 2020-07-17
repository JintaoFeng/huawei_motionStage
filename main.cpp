#include "mainwindow.h"
#include "axis.h"
#include <QApplication>
#include <axis.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
