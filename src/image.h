//
// Created by felix on 2/19/23.
//

#ifndef PICTURE_POSSUM_IMAGE_H
#define PICTURE_POSSUM_IMAGE_H

#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <regex>
#include <openssl/sha.h>
#include <set>
#include <QMetaType>
#include <utility>
#include <string>
#include <chrono>
#include <iomanip>
#include "ImageTypes.h"
#include "Tag.h"
#include <QJsonArray>

namespace possum {
    static const QString IMAGE_PATHES_KEY = "pathes";
    static const QString IMAGE_CHECKSUM_KEY = "checksum";
    static const QString IMAGE_TAGS_KEY = "tag_ids";
    static const QString IMAGE_CREATION_KEY = "creation_time";
    static const QString IMAGE_TYPE_KEY = "type";
    ///Type for describing images in the filesystem
    class Image {
    public:

///Getter for path
        [[nodiscard]] const std::filesystem::path &getPath() const;

        ///Getter for sha1sum
        [[nodiscard]] const std::string &getSha1Sum() const;

        [[nodiscard]] std::string getFilename() const;

        void add_path(const std::filesystem::path&);

        void add_tag(const Tag& tag);

        void clear_tags();

        ///getter for type
        [[nodiscard]] ImageType getType() const;

        ///Constructing Image data structure. sha1sum needs to be calculated beforehand.
        Image(const std::string& path, std::string sha1Sum, ImageType type, time_t creation_time);

        Image();

        void setCreationTime(time_t creationTime);

        [[nodiscard]] time_t getCreationTime() const;

        [[nodiscard]] QJsonObject to_json() const;
        static Image from_json(const QJsonObject&);

        [[nodiscard]] const std::set<std::string> &getTagIds() const;

        [[nodiscard]] bool is_empty() const;
    private:
        ///path of the image in the file system
        std::vector<std::filesystem::path> pathes;

        ///sha1 hash of the image contents
        std::string sha1_sum;

        ///associated tags
        std::set<std::string> tag_ids;

        ///Image type
        ImageType type;

        time_t creation_time;

        Image(const std::vector<std::string>& pathes, std::string  sha1Sum, ImageType type, time_t creation_time, const std::set<std::string>& tag_ids);

    };
}

#endif //PICTURE_POSSUM_IMAGE_H
