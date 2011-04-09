#include <QtGui/QApplication>
#include "mainwindow.h"
#include "nombrewizard.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    //MainWindow w;
    //w.show();
    NombreWizard w;
    w.show();
    return a.exec();
}
