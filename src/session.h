//
// Created by felix on 2/19/23.
//

#ifndef PICTURE_POSSUM_SESSION_H
#define PICTURE_POSSUM_SESSION_H

#include <filesystem>
#include <vector>
#include "image.h"

namespace possum {
    class Session {
        std::filesystem::path directory_path;
        std::vector<Image> images;
    public:
        explicit Session(const std::filesystem::path &directoryPath);
        Session();
    };
}
#endif //PICTURE_POSSUM_SESSION_H
