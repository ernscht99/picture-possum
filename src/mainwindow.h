#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include "session.h"
#include "imagesListModel.h"
#include <QMainWindow>
#include <QProgressDialog>
#include <QFileDialog>
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
        possum::ImagesListModel images_model;

    public slots:
        void load_folder();
    };
#endif // MAINWINDOW_H
