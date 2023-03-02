#include "mainwindow.h"

#include <QApplication>
#include "image.h"
#include <iostream>
int main(int argc, char *argv[])
{
    using namespace std;
    using namespace possum;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}
