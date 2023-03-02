#include "mainwindow.h"
#include "./ui_mainwindow.h"
    using namespace possum;
    MainWindow::MainWindow(QWidget *parent)
            : QMainWindow(parent), ui(new Ui::MainWindow){
        ui->setupUi(this);
        ui->listView->setModel(&this->images_model);
        connect(ui->actionOpen_Folder, SIGNAL(triggered(bool)), this, SLOT(load_folder()));
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

void MainWindow::load_folder() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Image Directory"), "/home");
    images_model.setImages(possum::Image::get_images(path.toStdString(), {Image::JPEG, Image::PNG}));

}

