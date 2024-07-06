//
// Created by felix on 3/7/23.
//

#include "Settings.h"


possum::Settings::Settings(const std::vector<Tag> &tag_vector, const std::set<possum::ImageType> &validTypes,
                           QStringList dateConversionFormats) : valid_types(validTypes), date_conversion_formats(std::move(dateConversionFormats)) {
    for (const Tag &t : tag_vector) {
        tags.insert({t.getIdentifier(), t});
    }
}

std::string possum::Settings::render_tag_symbol(const std::string &tag_id) const {
    auto tag_iterator = tags.find(tag_id);
    if(tag_iterator  != tags.end()) {
        return tag_iterator->second.emoji;
    }
    return "❓";
}

std::string possum::Settings::render_tag_full(const std::string &tag_id) const {
    auto tag_iterator = tags.find(tag_id);
    if(tag_iterator  != tags.end()) {
        return tag_iterator->second.emoji + " " + tag_iterator->second.name;
    }
    return "❓ "+ tag_id;

}

QJsonObject possum::Settings::to_json() const{
    QJsonObject root;
    QJsonArray valid_types_arr;
    for (ImageType valid_type : valid_types) {
        valid_types_arr.push_back(QJsonValue{valid_type});
    }
    QJsonArray tags_arr;
    for (const auto & [k, tag] : tags) {
       tags_arr.push_back(tag.to_json());
    }
    root[DATE_FORMATS_KEY]= QJsonArray::fromStringList(date_conversion_formats);
    root[VALID_TYPES_KEY]= valid_types_arr;
    root[TAGS_ARRAY_KEY]= tags_arr;
    return root;
}

possum::Settings possum::Settings::from_json(const QJsonObject & json) {
    std::vector<Tag> tags_arr;
    for (const auto & jsonTag : json[TAGS_ARRAY_KEY].toArray()) {
        tags_arr.emplace_back(Tag::from_json(jsonTag.toObject()));
    }
    std::set<ImageType> valid_types_arr;
    for (const auto & jsonImageType : json[VALID_TYPES_KEY].toArray()) {
        //Pfusch
        valid_types_arr.insert(static_cast<ImageType>(jsonImageType.toInt()));
    }
    QStringList date_formats_arr;
    for (const auto & jsonDateFormat : json[DATE_FORMATS_KEY].toArray()) {
        date_formats_arr.emplace_back(jsonDateFormat.toString());
    }
    return Settings{
            tags_arr,
            valid_types_arr,
            date_formats_arr
    };
}

std::string possum::Settings::render_tag_name(const std::string &tag_id) const {
    auto tag_iterator = tags.find(tag_id);
    if(tag_iterator  != tags.end()) {
        return tag_iterator->second.name;
    }
    return "";
}


