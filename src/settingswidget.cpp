#include "settingswidget.h"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(possum::Settings& settings, QWidget* parent) : QWidget(parent, Qt::Popup | Qt::Dialog), ui(new Ui::SettingsWidget),
                                                                              date_formats(settings.date_conversion_formats),
                                                                              tags(settings.tags) {
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &SettingsWidget::accept);
    connect(this, SIGNAL(save_settings(const possum::Settings&)), parent, SLOT(setSettings(const possum::Settings&)));
    connect(ui->date_format_delete, &QPushButton::clicked, this, &SettingsWidget::delete_date_format);
    connect(ui->date_format_add, &QPushButton::clicked, this, &SettingsWidget::add_date_format);
    connect(ui->tags_add, &QPushButton::clicked, this, &SettingsWidget::add_tag);
    connect(ui->tags_edit, &QPushButton::clicked, this, &SettingsWidget::edit_tag);
    connect(ui->tags_delete, &QPushButton::clicked, this, &SettingsWidget::delete_tag);
    setWindowTitle(tr("Settings"));
    ui->date_format_view->setModel(&date_formats);
    ui->tags_table->setModel(&tags);
    ui->tags_table->setFont(QFont("Emoji", 12));
    ui->checkbox_jpg->setChecked(settings.valid_types.contains(possum::JPEG));
    ui->checkbox_png->setChecked(settings.valid_types.contains(possum::PNG));
}

SettingsWidget::~SettingsWidget() {
    delete ui;
}

void SettingsWidget::accept(QAbstractButton* button) {
    switch (ui->buttonBox->standardButton(button)) {
        case (QDialogButtonBox::Save):
            save();
            break;
        case (QDialogButtonBox::Ok):
            save();
            close();
            break;
        case (QDialogButtonBox::Cancel):
        default:
            close();
    }
}

void SettingsWidget::save() {
    std::set<possum::ImageType> selected_types{};
    if (ui->checkbox_jpg->isChecked())
        selected_types.insert(possum::ImageType::JPEG);
    if (ui->checkbox_png->isChecked())
        selected_types.insert(possum::ImageType::PNG);

    possum::Settings settings{
        {tags.getTags()},
        selected_types,
        date_formats.stringList().toList()};
    save_settings(settings);
}

void SettingsWidget::delete_date_format() {
    QItemSelectionModel* selectionModel = ui->date_format_view->selectionModel();
    if (selectionModel->hasSelection()) {
        date_formats.removeRows(selectionModel->selectedRows()[0].row(), 1);
    }
}

void SettingsWidget::add_date_format() {
    auto row_index = date_formats.rowCount();
    date_formats.insertRows(row_index, 1);
    date_formats.setData(date_formats.index(row_index), {QString{"%Y-%m-%dT%H:%M:%SZ"}});
}

void SettingsWidget::add_tag() {
    auto x = new TagDialog{this};
    x->exec();
}

void SettingsWidget::edit_tag() {
    QItemSelectionModel* selectionModel = ui->tags_table->selectionModel();
    if (!selectionModel->hasSelection())
        return;
    auto x = new TagDialog{tags.getTag(selectionModel->currentIndex()), this};
    x->exec();
}

void SettingsWidget::delete_tag() {
    QItemSelectionModel* selectionModel = ui->tags_table->selectionModel();
    if (selectionModel->hasSelection()) {
        tags.removeRows(selectionModel->selectedRows()[0].row(), 1);
    }
}

void SettingsWidget::change_tag(const possum::Tag& itag) {
    tags.update_tag(tag);
}
