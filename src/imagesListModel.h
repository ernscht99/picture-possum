//
// Created by felix on 3/2/23.
//

#ifndef PICTURE_POSSUM_IMAGESLISTMODEL_H
#define PICTURE_POSSUM_IMAGESLISTMODEL_H
#include <QAbstractItemModel>
#include <QJsonObject>
#include <QJsonDocument>
#include "TinyEXIF.h"
#include "image.h"
#include "Settings.h"
#include <ctime>
#include <filesystem>
#include <QFile>
#include <QFont>

namespace possum {
    const static QString SETTINGS_KEY = "settings";
    const static QString IMAGE_LIST_KEY = "image_list";
    enum ImageListRoles {
        DataView = Qt::UserRole
    };
    class ImagesListModel : public QAbstractTableModel {
    Q_OBJECT

        std::vector<std::string> hash_handles;
        std::map<std::string, std::unique_ptr<Image>> image_map;
        possum::Settings settings;
        bool unsaved_changes;
        explicit ImagesListModel(Settings settings, std::map<std::string, std::unique_ptr<Image>> images, QObject *parent = nullptr);

    public:

        ///Must be implemented because QAbstractListModel
        explicit ImagesListModel(Settings settings, QObject *parent = nullptr);
        [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
        [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;

        [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

        ///Return a vector of Image objects corresponding to the contents of the directory at directory_path.
        ///Only includes images with valid extensions as specified in valid_types TODO make asynchronous
        void load_images(const std::string &directory_path);

        ///Insert Image into structure
        void insert_image(const Image&);

        ///Return Image that corresponds to a hash. Empty Image if there is none.
        Image get_image(const std::string& hash);

        void update_image(const Image& updater);

        void setSettings(const Settings &settings);

        static std::string get_exif_date(std::ifstream &file_stream);

        time_t estimate_date(std::ifstream &file_stream, const std::string &file_name);

        [[nodiscard]] QJsonObject to_json() const;

        static ImagesListModel from_json(const QJsonObject&);

        [[nodiscard]] bool save(const std::filesystem::path & path);
        void load(const std::filesystem::path & path);

        [[nodiscard]] const Settings &getSettings() const;

        [[nodiscard]] bool has_unsaved_changes() const;

    };
}

#endif //PICTURE_POSSUM_IMAGESLISTMODEL_H
