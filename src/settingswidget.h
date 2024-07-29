#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QAbstractButton>
#include <QDialogButtonBox>
#include "Settings.h"
#include <QStringListModel>
#include <QButtonGroup>
#include "TagListModel.h"
#include "tagdialog.h"

namespace Ui {
    class SettingsWidget;
}

class SettingsWidget : public QWidget {
Q_OBJECT

public:
    explicit SettingsWidget(possum::Settings &, QWidget *parent = nullptr);

    ~SettingsWidget();


public slots:

    void accept(QAbstractButton *);

    void save();

    void add_tag();

    void delete_tag();

    void edit_tag();

    signals:

    void save_settings(const possum::Settings &);

private:
    Ui::SettingsWidget *ui;
    QStringListModel date_formats;
    possum::TagListModel tags;

    void delete_date_format();

    void add_date_format();

};

#endif // SETTINGSWIDGET_H
