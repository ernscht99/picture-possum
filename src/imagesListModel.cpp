//
// Created by felix on 3/2/23.
//

#include "imagesListModel.h"

#include <utility>
namespace possum{
    ImagesListModel::ImagesListModel(std::vector<Image> images, QObject *parent) : QAbstractListModel(parent), images(std::move(images)){}

    QVariant ImagesListModel::data(const QModelIndex &index, int role) const {
        if (!index.isValid())
            return {};

        if (static_cast<unsigned long>(index.row()) >= images.size())
            return {};

        if (role == Qt::DisplayRole)
            return {QString::fromStdString(images.at(index.row()).getPath())};
        else
            return {};
    }

    QVariant ImagesListModel::headerData(int section, Qt::Orientation orientation, int role) const {
        return {};
    }

    int ImagesListModel::rowCount(const QModelIndex &parent) const {
        return static_cast<int>(images.size());
    }

    ImagesListModel::ImagesListModel(QObject *parent) : QAbstractListModel(parent), images(std::vector<Image>()) {
    }

    void ImagesListModel::setImages(const std::vector<Image> &images) {
        this->images = images;
        emit dataChanged(createIndex(0,0), createIndex(images.size()-1, 0));
    }

}
