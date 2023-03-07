//
// Created by felix on 3/7/23.
//

#ifndef PICTURE_POSSUM_SETTINGS_H
#define PICTURE_POSSUM_SETTINGS_H


#include <map>
#include "Tag.h"
#include "ImageTypes.h"
#include <set>
namespace possum {
    class Settings {
    public:
        std::map<std::string, Tag> tags;
        std::set<possum::ImageType> valid_types;
        std::vector<std::string> date_conversion_formats;

    Settings(
            const std::vector<Tag> &tag_vector,
            const std::set<possum::ImageType> &validTypes,
            const std::vector<std::string> &dateConversionFormats);
    };
}

#endif //PICTURE_POSSUM_SETTINGS_H
