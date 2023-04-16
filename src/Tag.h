//
// Created by felix on 3/7/23.
//

#ifndef PICTURE_POSSUM_TAG_H
#define PICTURE_POSSUM_TAG_H


#include <string>
#include <QKeySequence>
#include <QJsonObject>

static const QString TAG_EMOJI_KEY = "tag_emoji";
static const QString TAG_NAME_KEY = "tag_name";
static const QString TAG_IDENTIFIER_KEY = "tag_identifier";
static const QString TAG_KEYSEQUENCE_KEY = "tag_key_sequence";
class Tag {
private:
    std::string identifier;
public:
    std::string emoji;
    std::string name;
    QKeySequence key_sequence;
    Tag(std::string identifier, std::string emoji, std::string name, const QKeySequence& key_sequence);
    [[nodiscard]] QJsonObject to_json() const;
    static Tag from_json(const QJsonObject &);

    [[nodiscard]] const std::string &getIdentifier() const;
};


#endif //PICTURE_POSSUM_TAG_H
