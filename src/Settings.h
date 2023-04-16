//
// Created by felix on 3/7/23.
//

#ifndef PICTURE_POSSUM_SETTINGS_H
#define PICTURE_POSSUM_SETTINGS_H


#include <map>
#include "Tag.h"
#include "ImageTypes.h"
#include <set>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <utility>
#include <QJsonArray>
#include <QJsonValue>

namespace possum {
    static const QString DATE_FORMATS_KEY = "dates";
    static const QString VALID_TYPES_KEY = "valid_types";
    static const QString TAGS_ARRAY_KEY = "tags";

    class Settings{
    public:

        std::map<std::string, Tag> tags;
        std::set<possum::ImageType> valid_types;
        QStringList date_conversion_formats;
        [[nodiscard]] std::string render_tag_symbol(const std::string & tag_id) const;
        [[nodiscard]] std::string render_tag_full(const std::string & tag_id) const;
        [[nodiscard]] QJsonObject to_json() const;
        static Settings from_json(const QJsonObject&);


        Settings(
            const std::vector<Tag> &tag_vector,
            const std::set<possum::ImageType> &validTypes,
            QStringList dateConversionFormats);

    };
}

#endif //PICTURE_POSSUM_SETTINGS_H
