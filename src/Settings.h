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
    ///Keys for json export
    static const QString DATE_FORMATS_KEY = "dates";
    static const QString VALID_TYPES_KEY = "valid_types";
    static const QString TAGS_ARRAY_KEY = "tags";

    ///Represents the Settings of a session
    class Settings{
    public:
        ///Contains all defined tags, with the tag_identifier as key
        std::map<std::string, Tag> tags;

        ///Contains all valid, i.e. considered types when loading a new directory
        std::set<possum::ImageType> valid_types;

        ///Contains format strings for guessing the creation date from the filename (According to strptime)
        QStringList date_conversion_formats;


        ///Return the symbol of the tag associated with the tag_id
        [[nodiscard]] std::string render_tag_symbol(const std::string & tag_id) const;

        ///Return name of the tag associated with the tag_id
        [[nodiscard]] std::string render_tag_name(const std::string & tag_id) const;

        ///Return concatination of symbol and name jAof the tag associated with the tag_id
        [[nodiscard]] std::string render_tag_full(const std::string & tag_id) const;

        ///Convert the Settings object to a JSON representation
        [[nodiscard]] QJsonObject to_json() const;

        ///Parse a Settings object from its JSON representation
        static Settings from_json(const QJsonObject&);


        ///Construct a settings object from a list of tags, valid types, and conversion formats
        Settings(
            const std::vector<Tag> &tag_vector,
            const std::set<possum::ImageType> &validTypes,
            QStringList dateConversionFormats);

    };
}

#endif //PICTURE_POSSUM_SETTINGS_H
