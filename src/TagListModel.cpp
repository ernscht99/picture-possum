#include "TagListModel.h"

possum::TagListModel::TagListModel(const std::map<std::string, Tag> &tag_map, QObject *parent) : QAbstractTableModel(
        parent) {
    for (const auto &kv: tag_map) {
        tags.emplace_back(kv.second);
    }
}

int possum::TagListModel::rowCount(const QModelIndex &) const {
    return static_cast<int>(tags.size());
}

QVariant possum::TagListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return {};
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0:
                return {QString::fromStdString(tags[index.row()].emoji)};
            case 1:
                return {QKeySequence{tags[index.row()].key_sequence}};
            case 2:
            default:
                return {QString::fromStdString(tags[index.row()].name)};
        }
    }
    return {};
}

QVariant possum::TagListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return {};
    if (orientation == Qt::Horizontal)
        switch (section) {
            case 0:
                return {QString("Symbol")};
            case 1:
                return {QString("Shortcut")};
            case 2:
                return {QString("Title")};
            default:
                return {};
        }
    return {};

}

int possum::TagListModel::columnCount(const QModelIndex &) const {
    return 3;
}

bool possum::TagListModel::removeRows(int row, int count, const QModelIndex &) {
    if (row + count - 1 < rowCount()) {
        tags.erase(tags.begin() + row, tags.begin() + row + count);
        layoutChanged();
        return true;
    }
    return false;
}

const possum::Tag &possum::TagListModel::getTag(const QModelIndex &index) const {
    return tags[index.row()];
}

void possum::TagListModel::update_tag(const Tag &new_tag) {
    for (auto &tag: tags) {
        if (tag.getIdentifier() == new_tag.getIdentifier()) {
            tag.key_sequence = new_tag.key_sequence;
            tag.name = new_tag.name;
            tag.emoji = new_tag.emoji;
            layoutChanged();
            return;
        }
    }
    tags.emplace_back(new_tag);
    layoutChanged();
}

const std::vector<possum::Tag> &possum::TagListModel::getTags() {
    return tags;
}
