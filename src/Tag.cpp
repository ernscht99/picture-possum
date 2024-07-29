#include "Tag.h"

namespace possum {
    Tag::Tag(std::string identifier, std::string emoji, std::string name, const QKeySequence &key_sequence)
            : identifier(std::move(identifier)), emoji(std::move(emoji)), name(std::move(name)),
              key_sequence(key_sequence) {}

    const std::string &Tag::getIdentifier() const {
        return identifier;
    }

    QJsonObject Tag::to_json() const {
        QJsonObject root{};
        root[TAG_EMOJI_KEY] = QJsonValue{QString::fromStdString(emoji)};
        root[TAG_NAME_KEY] = QJsonValue{QString::fromStdString(name)};
        root[TAG_IDENTIFIER_KEY] = QJsonValue{QString::fromStdString(identifier)};
        root[TAG_KEYSEQUENCE_KEY] = QJsonValue{key_sequence.toString()};
        return root;
    }

    Tag Tag::from_json(const QJsonObject &json) {
        return Tag{json[TAG_IDENTIFIER_KEY].toString().toStdString(),
                   json[TAG_EMOJI_KEY].toString().toStdString(),
                   json[TAG_NAME_KEY].toString().toStdString(),
                   QKeySequence::fromString(json[TAG_KEYSEQUENCE_KEY].toString())};
    }
} // namespace possum