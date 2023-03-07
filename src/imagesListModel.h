//
// Created by felix on 3/2/23.
//

#ifndef PICTURE_POSSUM_IMAGESLISTMODEL_H
#define PICTURE_POSSUM_IMAGESLISTMODEL_H
#include <QAbstractItemModel>
#include "TinyEXIF.h"
#include "image.h"
#include "Settings.h"
#include <time.h>

namespace possum {
    class ImagesListModel : public QAbstractListModel {
    Q_OBJECT

        std::vector<std::string> hash_handles;
        std::map<std::string, std::unique_ptr<Image>> image_map;

    public:

        ///Must be implemented because QAbstractListModel
        explicit ImagesListModel(QObject *parent = nullptr);
        [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
        [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;


        ///Return a vector of Image objects corresponding to the contents of the directory at directory_path.
        ///Only includes images with valid extensions as specified in valid_types TODO make asynchronous
        void load_images(const std::string& directory_path, const Settings &settings);

        ///Insert Image into structure
        void insert_image(const Image&);

        Image get_image(const std::string& hash);

        void update_image(const Image& updater);

        static std::string get_exif_date(std::ifstream &file_stream);

        static time_t estimate_date(std::ifstream &file_stream, const Settings &settings, const std::string &file_name);
    };
}

#endif //PICTURE_POSSUM_IMAGESLISTMODEL_H
