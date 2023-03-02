//
// Created by felix on 3/2/23.
//

#ifndef PICTURE_POSSUM_IMAGESLISTMODEL_H
#define PICTURE_POSSUM_IMAGESLISTMODEL_H
#include <QAbstractItemModel>
#include "image.h"
namespace possum {
    class ImagesListModel : public QAbstractListModel {
    Q_OBJECT

        std::vector<Image> images;
    public:
        explicit ImagesListModel(QObject *parent = nullptr);
        explicit ImagesListModel(std::vector<Image> images, QObject *parent = nullptr);
        [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
        [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;

        void setImages(const std::vector<Image> &images);
    };
}

#endif //PICTURE_POSSUM_IMAGESLISTMODEL_H
