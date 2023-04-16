//
// Created by felix on 3/7/23.
//

#ifndef PICTURE_POSSUM_IMAGETYPES_H
#define PICTURE_POSSUM_IMAGETYPES_H

#include <string>
#include <regex>
#include <QString>
///File Types corresponding to file extensions, limited to supported types
namespace possum {
    enum ImageType {
        JPEG, PNG, BMP, Other, None
    };

    ImageType parse_extension(const std::string& file_path);
}
#endif //PICTURE_POSSUM_IMAGETYPES_H
