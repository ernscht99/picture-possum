#include "mainwindow.h"
#include "./ui_mainwindow.h"

    namespace {
    QString time_to_string(time_t time) {
        time_t creation_time = time;
        return QString(ctime(&creation_time)).chopped(1);
    }
    }
    using namespace possum;
    MainWindow::MainWindow(QWidget *parent)
            : QMainWindow(parent), ui(new Ui::MainWindow),
            settings({
                {"family", "👨‍👩‍👦", "Familie", {250, 128, 114}},
                {"family", "👨‍👩‍👦", "Familie", {250, 128, 114}}},
                     { JPEG, PNG},
                     {"%Y-%m-%d", "%Y%m%d"}
            )
                     {
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
        images_model.load_images(path.toStdString(), settings);
    }
}

void MainWindow::display_image(const QModelIndex & index){
        picture_scene.clear();
        auto data = images_model.data(index, 0);
        std::string hash{data.toString().toStdString()};
        Image image = images_model.get_image(hash);
        ui->label_file_name->setText(QString::fromStdString(image.getFilename()));
        ui->label_sha1->setText(data.toString());
        ui->label_exif->setText(time_to_string(image.getCreationTime()));
        QPixmap image_data(image.getPath().c_str());
        image_data = image_data.scaled(ui->graphicsView->size() * 0.95, Qt::KeepAspectRatio);
        //picture_scene.setBackgroundBrush(image_data);
        picture_scene.addPixmap(image_data);
}