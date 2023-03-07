#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include "session.h"
#include "imagesListModel.h"
#include "ImageTypes.h"
#include <QMainWindow>
#include <QProgressDialog>
#include <QFileDialog>
#include <QGraphicsScene>

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

    public slots:
        void load_folder();
        void display_image(const QModelIndex &);
    };
#endif // MAINWINDOW_H
