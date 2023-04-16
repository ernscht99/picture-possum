#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QPushButton>
#include "Tag.h"

namespace Ui {
class TagDialog;
}

class TagDialog : public QDialog
{
    Q_OBJECT

public:
    constexpr static const std::string symbols[]{
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
    };
    explicit TagDialog(QWidget *parent = nullptr);
    explicit TagDialog(const Tag& tag, QWidget *parent = nullptr);
    ~TagDialog() override;
    signals:
    void change_tag(const Tag & tag);
    public slots:
    void save();

private:
    Ui::TagDialog *ui;
    QButtonGroup symbol_buttons_group;
    std::vector<QPushButton *> symbol_buttons;
    std::string identifier;
    [[nodiscard]] QAbstractButton* find_button(const std::string& symbol) const;
};

#endif // TAGDIALOG_H
