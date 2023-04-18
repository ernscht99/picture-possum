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

        ///Get the first filename
        [[nodiscard]] std::string getFilename() const;

        ///Add a path to that image on disk
        void add_path(const std::filesystem::path&);

        ///Add a tag to this image
        void add_tag(const Tag& tag);

        ///Delete all associated tags
        void clear_tags();

        ///getter for type
        [[nodiscard]] ImageType getType() const;

        ///Constructing Image data structure. sha1sum needs to be calculated beforehand.
        Image(const std::string& path, std::string sha1Sum, ImageType type, time_t creation_time);

        ///Construct a dummy image, for display when program is first opened
        Image();

        ///set the creation time
        void setCreationTime(time_t creationTime);

        ///get the creation time
        [[nodiscard]] time_t getCreationTime() const;

        ///Convert this Image object into its JSON representation
        [[nodiscard]] QJsonObject to_json() const;

        ///Parse Image object from a JSON representation
        static Image from_json(const QJsonObject&);

        ///Return the identifiers of all associated tags
        [[nodiscard]] const std::set<std::string> &getTagIds() const;

    private:
        ///path of the image in the file system
        std::vector<std::filesystem::path> pathes;

        ///sha1 hash of the image contents
        std::string sha1_sum;

        ///associated tags
        std::set<std::string> tag_ids;

        ///Image file type
        ImageType type;

        ///Estimated creation time
        time_t creation_time;

        ///Constructor for copying an Image, used when parsing from json
        Image(const std::vector<std::string>& pathes, std::string  sha1Sum, ImageType type, time_t creation_time, const std::set<std::string>& tag_ids);

    };
}

#endif //PICTURE_POSSUM_IMAGE_H
