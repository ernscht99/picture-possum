#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "settingswidget.h"

namespace {
    ///convert a unix timestapm to a QString
    QString time_to_string(time_t time) {
        time_t creation_time = time;
        return QString(ctime(&creation_time)).chopped(1);
    }

    ///Some default settings
    possum::Settings default_settings{{
                                              {"trash", "A", "Trash", Qt::Key_T},
                                              {"family", "👨", "Familie", Qt::Key_F}},
                                      {       possum::JPEG, possum::PNG},
                                      {       "%Y-%m-%d",   "%Y%m%d",}
    };
}
using namespace possum;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), settings(default_settings),
          images_model(settings) {
    setSettings(settings);
    ///Intitalize UI
    ui->setupUi(this);
    QFont font{"Noto"};
    font.setPixelSize(16);
    setFont(font);
    setWindowTitle(tr("Picture Possum"));

    ///Initialize Image List
    ui->tableView->setModel(&this->images_model);
    ///Initilize Image Viewer
    ui->graphicsView->setScene(&picture_scene);

    ///Connect Signals
    connect(ui->actionOpen_Folder, SIGNAL(triggered(bool)), this, SLOT(load_folder()));
    connect(ui->actionOpen_Settings, SIGNAL(triggered(bool)), this, SLOT(open_settings()));
    connect(ui->actionOpen_Session_File, SIGNAL(triggered(bool)), this, SLOT(load_file()));
    connect(ui->actionSave_Session_File, SIGNAL(triggered(bool)), this, SLOT(save_file()));

    ///Display Image when it's selected
    connect(ui->tableView->selectionModel(),
            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this,
            SLOT(display_image(const QModelIndex &)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::load_folder() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Image Directory"),
                                                     "/home/felix/Projects/picture_possum/test/test_dirs");
    if (!path.isEmpty()) {
        images_model.load_images(path.toStdString());
    }
}

void MainWindow::open_settings() {
    auto *s = new SettingsWidget(settings, this);
    s->setWindowModality(Qt::WindowModal);
    s->show();
}

void MainWindow::display_image(const QModelIndex &index) {
    ///Save previous picture
    images_model.update_image(current_image);

    ///Get new picture
    auto data = images_model.data(index, ImageListRoles::DataView);
    std::string hash{data.toString().toStdString()};
    current_image = images_model.get_image(hash);

    ///render it
    ui->label_file_name->setText(QString::fromStdString(current_image.getFilename()));
    ui->label_sha1->setText(data.toString());
    ui->label_exif->setText(time_to_string(current_image.getCreationTime()));
    paint_image();
}

void MainWindow::setSettings(const Settings &settings) {
    this->settings = settings;
    this->images_model.setSettings(settings);

    ///Set the global shortcuts based on the tags in the settings
    shortcuts.clear();
    auto delete_tags = [this]() {
        this->current_image.clear_tags();
        this->images_model.update_image(current_image);
    };
    shortcuts.emplace_back(std::make_unique<QShortcut>(QKeySequence{"Backspace"},this,delete_tags));

    auto move_down = [this]() {
        this->ui->tableView->selectRow(this->ui->tableView->currentIndex().row()+1);
    };
    shortcuts.emplace_back(std::make_unique<QShortcut>(QKeySequence{"Return"},this,move_down));
    shortcuts.emplace_back(std::make_unique<QShortcut>(QKeySequence{"Space"},this,move_down));

    for ( const auto & [key, value] : settings.tags) {
        auto tag_image_with = [this, value]() {
            this->current_image.add_tag(value);
            this->images_model.update_image(current_image);
        };
        shortcuts.emplace_back(std::make_unique<QShortcut>(value.key_sequence, this, tag_image_with));
    }
}

void MainWindow::paint_image() {
    picture_scene.clear();
    QPixmap image_data(current_image.getPath().c_str());
    image_data = image_data.scaled(ui->graphicsView->size() * 0.95, Qt::KeepAspectRatio);
    picture_scene.addPixmap(image_data);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    paint_image();
}

void MainWindow::save_file() {
    QString path_string = QFileDialog::getSaveFileName(this, "Save Session File", "/home", "*.json");
    if (path_string.isEmpty())
        return;
    if(!path_string.endsWith(".json"))
        path_string.append(".json");
    if (!images_model.save(std::filesystem::path{path_string.toStdString()})) {
        QMessageBox msgBox;
        msgBox.setText("Session could not be saved");
        msgBox.exec();
    }
}

void MainWindow::load_file() {
    QString path_string = QFileDialog::getOpenFileName(this, "Load Session File", "", "*.json");
    if (path_string.isEmpty())
        return;
    try {
        this->images_model.load(std::filesystem::path{path_string.toStdString()});
        this->setSettings(this->images_model.getSettings());
    } catch (...) {
        QMessageBox msgBox;
        msgBox.setText("Session could not be loaded");
        msgBox.exec();
    }
}
