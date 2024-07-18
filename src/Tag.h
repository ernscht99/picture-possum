#ifndef PICTURE_POSSUM_TAG_H
#define PICTURE_POSSUM_TAG_H

#include <string>
#include <QKeySequence>
#include <QJsonObject>

namespace possum {
    static const QString TAG_EMOJI_KEY = "tag_emoji";
    static const QString TAG_NAME_KEY = "tag_name";
    static const QString TAG_IDENTIFIER_KEY = "tag_identifier";
    static const QString TAG_KEYSEQUENCE_KEY = "tag_key_sequence";

    class Tag {
    private:
        ///Unique identifier for the tag, not visible to the user
        std::string identifier;
    public:
        ///Emoji representing the tag
        std::string emoji;

        ///User-visible name of the tag
        std::string name;

        ///Keyboard shortcut for tagging an image
        QKeySequence key_sequence;

        ///Constructor
        Tag(std::string identifier, std::string emoji, std::string name, const QKeySequence &key_sequence);

        ///Convert the tag to a json Object representation
        [[nodiscard]] QJsonObject to_json() const;

        ///Reconstruct a tag from a json Object representation
        static Tag from_json(const QJsonObject &);

        ///Getter for Identifier
        [[nodiscard]] const std::string &getIdentifier() const;
    };
}


#endif //PICTURE_POSSUM_TAG_H
