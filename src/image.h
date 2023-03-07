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
namespace possum {
    ///Type for describing images in the filesystem
    class Image {
    public:

        ///Getter for path
        [[nodiscard]] const std::filesystem::path &getPath() const;

        ///Getter for sha1sum
        [[nodiscard]] const std::string &getSha1Sum() const;

        [[nodiscard]] std::string getFilename() const;

        void add_path(const std::filesystem::path&);

        ///getter for type
        [[nodiscard]] ImageType getType() const;

        ///Constructing Image data structure. sha1sum needs to be calculated beforehand.
        Image(const std::string& path, std::string sha1Sum, ImageType type, time_t creation_time);

        void setCreationTime(time_t creationTime);

        [[nodiscard]] time_t getCreationTime() const;

    private:
        ///path of the image in the file system
        std::vector<std::filesystem::path> pathes;

        ///sha1 hash of the image contents
        std::string sha1_sum;

        ///Image type
        ImageType type;

        time_t creation_time;

    };
}

#endif //PICTURE_POSSUM_IMAGE_H
