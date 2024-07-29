#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QPushButton>
#include "Tag.h"

namespace Ui {
    class TagDialog;
}

///This dialog is shown to the user to create or edit a Tag
class TagDialog : public QDialog {
Q_OBJECT

public:
    ///A list of valid symbols that can be chosen for a tag
    constexpr static const char *symbols[]{ //cannot be strings because that breaks builds on Mac
            "❓",
            "❌",
            "⭕",
            "👨",
            "❤️",
            "🔥",
            "💀",
            "🐀",
            "⭐",
            "😂",
            "🥵",
            "🤓",
            "📷",
    };

    ///For creating a new Tag
    explicit TagDialog(QWidget *parent = nullptr);

    ///For editing an existing tag
    explicit TagDialog(const possum::Tag &tag, QWidget *parent = nullptr);

    ~TagDialog() override;

signals:

    ///Signals that the tag has been accepted to another widget
    void tag_changed(const possum::Tag &tag);

public slots:

    ///Save the currently edited tag. Sends out tag_changed
    void save();

private:
    Ui::TagDialog *ui;
    ///Exclusive Button group for choosing a symbol
    QButtonGroup symbol_buttons_group;

    ///Vector of buttons for choosing a symbol. All part of symbol_buttons_group
    std::vector<QPushButton *> symbol_buttons;

    ///The tag_identifier of the tag currently being edited
    std::string identifier;

    ///Returns pointer to the button representing symbol, if it exists. else returns first button
    [[nodiscard]] QAbstractButton *find_button(const std::string &symbol) const;
};

#endif // TAGDIALOG_H
