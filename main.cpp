#include "mainwindow.h"

#include <QApplication>
#include "image.h"
#include <iostream>
int main(int argc, char *argv[])
{
    using namespace std;
    using namespace possum;
    vector<Image> images{Image::get_images("/home/felix/Projects/picture_possum/test/test_dirs/dir1", {Image::Type::JPEG})};
    cout << images[0].getPath();
    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
     */

}
