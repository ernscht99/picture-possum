#ifndef PICTURE_POSSUM_IMAGETYPES_H
#define PICTURE_POSSUM_IMAGETYPES_H

#include <string>
#include <regex>
#include <QString>
///File Types corresponding to file extensions, limited to supported types
namespace possum {
    enum ImageType : uint16_t {
        JPEG, PNG, BMP, Other, None
    };

    ///Associate the enum parsed from a file name
    ImageType parse_extension(const std::string &file_path);
} // namespace possum
#endif //PICTURE_POSSUM_IMAGETYPES_H
