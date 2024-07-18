#include "tagdialog.h"
#include "./ui_tagdialog.h"

namespace {
    bool not_alphanumeric(char c) {
        return not (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9'));
    }
    std::string generate_tag_identifier(std::string title) {
        title.erase(std::remove_if(title.begin(), title.end(), not_alphanumeric), title.end());
        return title;
    }
}
TagDialog::TagDialog(QWidget *parent):
QDialog(parent),
ui(new Ui::TagDialog)
{
    ui->setupUi(this);
    for (const auto& symbol : symbols) {
        auto new_button = new QPushButton(QString::fromStdString(symbol), this);
        symbol_buttons.emplace_back(new_button);
        new_button->setCheckable(true);
        new_button->setFont(QFont("Emoji", 16));
        new_button->setFlat(true);
        new_button->setFixedSize(30,30);
        symbol_buttons_group.addButton(new_button);
        symbol_buttons_group.setExclusive(true);
        ui->symbol_layout->addWidget(new_button);
    }
    setWindowTitle(QString{"New Tag"});
    symbol_buttons_group.buttons()[0]->setChecked(true);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &TagDialog::save);
    connect(this, SIGNAL(tag_changed(const Tag&)), parent, SLOT(change_tag(const Tag &)));

}

TagDialog::~TagDialog()
{
    delete ui;
}

TagDialog::TagDialog(const possum::Tag &tag, QWidget *parent) : TagDialog(parent) {
    find_button(tag.emoji)->setChecked(true);
    ui->key_edit->setKeySequence(QKeySequence(tag.key_sequence));
    ui->title_edit->setText(QString::fromStdString(tag.name));
    identifier = tag.getIdentifier();
    setWindowTitle(QString::fromStdString(tag.getIdentifier()));
}

QAbstractButton * TagDialog::find_button(const std::string& symbol) const {
    for (auto button : symbol_buttons) {
        auto button_text = button->text().toStdString();
        if (button_text == symbol) {
            return button;
        }
    }
    return symbol_buttons[0];
}

void TagDialog::save() {
    std::string title = ui->title_edit->text().toStdString();
    possum::Tag tag{
        identifier.empty() ? generate_tag_identifier(title):identifier,
            symbol_buttons_group.checkedButton()->text().toStdString(),
            title,
            ui->key_edit->keySequence()
        };
    tag_changed(tag);
}