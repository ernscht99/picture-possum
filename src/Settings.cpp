//
// Created by felix on 3/7/23.
//

#include "Settings.h"

possum::Settings::Settings(const std::vector<Tag> &tag_vector, const std::set<possum::ImageType> &validTypes,
                           const std::vector<std::string> &dateConversionFormats) : valid_types(validTypes), date_conversion_formats(dateConversionFormats) {
    for (const Tag &t : tag_vector) {
        tags.insert({t.identifier, t});
    }
}
