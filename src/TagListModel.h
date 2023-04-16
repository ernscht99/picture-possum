//
// Created by felix on 3/18/23.
//

#ifndef PICTURE_POSSUM_TAGLISTMODEL_H
#define PICTURE_POSSUM_TAGLISTMODEL_H

#include <QAbstractTableModel>
#include "Tag.h"
#include <vector>

namespace possum {
    class TagListModel : public QAbstractTableModel {
    Q_OBJECT
    std::vector<Tag> tags;
    public:

        ///Must be implemented because QAbstractListModel
        explicit TagListModel(const std::map<std::string, Tag>& tag_map, QObject *parent = nullptr);
        [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
        [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
                                          int role = Qt::DisplayRole) const override;

        [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        [[nodiscard]] const Tag & getTag(const QModelIndex &index) const;
        void update_tag(const Tag & tag);
        const std::vector<Tag> & getTags();

    };
}


#endif //PICTURE_POSSUM_TAGLISTMODEL_H
