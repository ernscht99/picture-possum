//
// Created by felix on 2/19/23.
//

#include "session.h"

namespace possum {
    Session::Session(const std::filesystem::path &directoryPath) :
    directory_path(directoryPath),
    images(Image::get_images(directoryPath, std::set<Image::Type>{Image::JPEG})) {}

    Session::Session() : directory_path(), images() {}
}