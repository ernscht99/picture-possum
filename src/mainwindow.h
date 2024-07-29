#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include "imagesListModel.h"
#include "ImageTypes.h"
#include <QMainWindow>
#include <QProgressDialog>
#include <QKeyEvent>
#include <QFileDialog>
#include <QFontDatabase>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    ///Object representing the current settings
    possum::Settings settings;

    ///Container for the loaded images
    possum::ImagesListModel images_model;

    ///UI element for displaying a single pictures
    QGraphicsScene picture_scene;

    ///The currently displayed image
    possum::Image current_image;

    ///Vector of shortcuts for quickly tagging images
    std::vector<std::unique_ptr<QShortcut>> shortcuts;

    ///display the current image in the UI container
    void paint_image();

    ///Open a dialog asking if unsaved changes should be saved. Returns if user want to proceed
    bool ask_about_unsaved();

protected:
    ///Resize the displayed image when the size of the main window changes
    void resizeEvent(QResizeEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

public slots:

    ///Open a dialog to open a directory that contains images
    void load_dir();

    ///Load an image from the image container to be displayed
    void display_image(const QModelIndex &);

    ///Open a widget for opening settings
    void open_settings();

    ///Load a session file
    void load_file();

    ///Save a session file
    void save_file();

    void generate_sorted_dir();

    ///Update the new_settings.
    void setSettings(const possum::Settings &new_settings);

};

#endif // MAINWINDOW_H
