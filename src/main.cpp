#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace possum;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();

}
