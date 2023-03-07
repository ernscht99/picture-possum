//
// Created by felix on 2/19/23.
//
#include "image.h"


namespace possum {

    Image::Image(const std::string& path, std::string sha1Sum, ImageType type, time_t creation_time) :
    sha1_sum(std::move(sha1Sum)), type(type), creation_time(creation_time) {
        pathes.emplace_back(path);
    }

    const std::filesystem::path &Image::getPath() const {
        return pathes.at(0);
    }


    const std::string &Image::getSha1Sum() const {
        return sha1_sum;
    }

    ImageType Image::getType() const {
        return type;
    }

    void Image::add_path(const std::filesystem::path &new_path) {
        pathes.emplace_back(new_path);
    }

    std::string Image::getFilename() const {
        auto a = getPath().filename();
        return getPath().filename().string();
    }

    void Image::setCreationTime(time_t creationTime) {
        creation_time = creationTime;
    }

    time_t Image::getCreationTime() const {
        return creation_time;
    }

}