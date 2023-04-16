#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include "session.h"
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
        MainWindow(QWidget *parent = nullptr);

        ~MainWindow();

    private:
        Ui::MainWindow *ui;
        possum::Settings settings;
        possum::ImagesListModel images_model;
        QGraphicsScene picture_scene;
        possum::Image current_image;
        std::vector<std::unique_ptr<QShortcut>> shortcuts;

        void paint_image();

    protected:
        void resizeEvent(QResizeEvent *event) override;

    public slots:
        void load_folder();
        void display_image(const QModelIndex &);

        void open_settings();

        void load_file();
        void save_file();
        void setSettings(const possum::Settings &settings);
    };
#endif // MAINWINDOW_H
