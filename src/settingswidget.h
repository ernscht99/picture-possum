#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QStringListModel>
#include <QButtonGroup>
#include "Settings.h"
#include "TagListModel.h"
#include "tagdialog.h"

namespace Ui {
    class SettingsWidget;
} // namespace Ui

class SettingsWidget : public QWidget {
Q_OBJECT

public:
    explicit SettingsWidget(possum::Settings &, QWidget *parent = nullptr);

    ~SettingsWidget() override;


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
