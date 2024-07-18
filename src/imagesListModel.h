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
#include "KeyGenerator.h"

///How many threads are used when processing images for duplicate detection
#define DUPLICATION_DETECTION_THREADS 4

namespace possum {
    const static QString SETTINGS_KEY = "settings";
    const static QString IMAGE_LIST_KEY = "image_list";

    enum ImageListRoles {
        DataView = Qt::UserRole
    };

    ///Container for Image Objects that can be displayed as a Table in QT, allowing efficient duplicate detection
    class ImagesListModel : public QAbstractTableModel {
    Q_OBJECT

        ///All keys of the hash map are stored again in this vector to allow constructing a table that has an order
        ///(see the data() method)
        std::vector<std::string> hash_handles;

        ///Principal container for Image objects. The key is a string that represents the content of an image,
        ///such as a hash over the entire image file, a hash over the pixel values, or some other value.
        ///If an new Image object is inserted that has the same or similar content as another Image that is already
        ///contained, it has the same key, and its path is simply appended to the Image that already exists in the
        ///map (see insert_image() and KeyGenerator.h)
        std::map<std::string, std::unique_ptr<Image>> image_map;

        ///Copy of the settings handed down from the main window
        possum::Settings settings;

        ///Flag indicating if images have been updated with e.g. new tags that have not yet been saved as a json export
        bool unsaved_changes;

        ///Constructor
        explicit ImagesListModel(Settings settings, std::map<std::string, std::unique_ptr<Image>> images, QObject *parent = nullptr);

    public:

        ///Must be implemented because QAbstractListModel
        explicit ImagesListModel(Settings settings, QObject *parent = nullptr);
        [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
        [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;
        [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        ///Sort the Images by some feature indicated by the column
        void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

        ///Return a vector of Image objects corresponding to the contents of the directory at directory_path.
        ///Only includes images with valid extensions as specified in valid_types TODO make asynchronous
        void load_images(const std::string &directory_path);

        ///Insert Image into structure
        void insert_image(const Image&);

        ///Return Image that corresponds to a hash. Empty Image if there is none.
        Image get_image(const std::string& hash);

        ///Update an existing image
        void update_image(const Image& updater);

        ///Setter for the settings
        void setSettings(const Settings &settings);

        ///Getter for settings
        [[nodiscard]] const Settings &getSettings() const;

        ///Try to extract the Creation date from the EXIF metadata of a JPEG file. Returns "" if there is none.
        static std::string get_exif_date(std::ifstream &file_stream);

        ///Trys to estimate the creation time of an Image first from the EXIF data if applicable, then from the file
        ///name. Returns 0 if this fails.
        time_t estimate_date(std::ifstream &file_stream, const std::string &file_name);

        ///Represent the state of the structure as a json Object
        [[nodiscard]] QJsonObject to_json() const;

        ///Reconstruct the state of a structure from a json Object
        static ImagesListModel from_json(const QJsonObject&);

        ///Write the state of the structure to disk as a json file
        [[nodiscard]] bool save(const std::filesystem::path & path);

        ///Reconstruct the state of the structure from a json file
        void load(const std::filesystem::path & path);

        ///Getter for the unsaved changes flag
        [[nodiscard]] bool has_unsaved_changes() const;

        ///Create an index structure comprised of symlinks for the Images on the disk
        void generate_sorted_dir(const std::filesystem::path & path);

        ///Delete all Image Objects from the structure
        void clear();
    };
}

#endif //PICTURE_POSSUM_IMAGESLISTMODEL_H
