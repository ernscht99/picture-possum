#include "mainwindow.h"
#include "./ui_mainwindow.h"
    using namespace possum;
    MainWindow::MainWindow(QWidget *parent)
            : QMainWindow(parent), ui(new Ui::MainWindow){
        ui->setupUi(this);
        ui->listView->setModel(&this->images_model);
        ui->graphicsView->setScene(&picture_scene);
        connect(ui->actionOpen_Folder, SIGNAL(triggered(bool)), this, SLOT(load_folder()));
        connect(ui->listView->selectionModel(),
                SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this,
                SLOT(display_image(const QModelIndex &)));
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

void MainWindow::load_folder() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Image Directory"), "/home/felix/Projects/picture_possum/test/test_dirs");
    if(!path.isEmpty()) {
        images_model.setImages(possum::Image::get_images(path.toStdString(), {Image::JPEG, Image::PNG}));
    }
}

void MainWindow::display_image(const QModelIndex & index){
        picture_scene.clear();
        auto data = images_model.data(index, 0);
        ui->label_file_name->setText(data.toString());
        QPixmap image_data(data.toString());
        image_data = image_data.scaled(ui->graphicsView->size() * 0.95, Qt::KeepAspectRatio);
        //picture_scene.setBackgroundBrush(image_data);
        picture_scene.addPixmap(image_data);
}