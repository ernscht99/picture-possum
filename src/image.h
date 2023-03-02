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

namespace possum {
    ///Type for describing images in the filesystem
    class Image {
    public:
        ///File Types corresponding to file extensions, limited to supported types
        enum Type{
            JPEG, PNG, BMP, Other, None
        };

        ///Getter for path
        [[nodiscard]] const std::string &getPath() const;

        ///Getter for sha1sum
        [[nodiscard]] const std::vector<unsigned char> &getSha1Sum() const;

        ///getter for type
        [[nodiscard]] Type getType() const;

        ///Constructing Image data structure. sha1sum needs to be calculated beforehand.
        Image(std::string path, const std::vector<unsigned char> &sha1Sum, Image::Type type);

        ///Return a vector of Image objects corresponding to the contents of the directory at directory_path.
        ///Only includes images with valid extensions as specified in valid_types TODO make asynchronous
        static std::vector<Image> get_images(const std::string& directory_path, const std::set<Image::Type> &valid_types);

    private:
        ///path of the image in the file system
        std::string path;

        ///sha1 hash of the image contents
        std::vector<unsigned char> sha1_sum;

        ///Image type
        Image::Type type;

        ///Parse the extension of a file to a Image::Type
        static Image::Type parse_extension(const std::string & file_path);
    };

}

#endif //PICTURE_POSSUM_IMAGE_H
