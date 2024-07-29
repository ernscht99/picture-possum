#include "./image.h"

namespace possum {

    Image::Image(const std::string& path, std::string sha1Sum, ImageType type, time_t creation_time) :
    sha1_sum(std::move(sha1Sum)), type(type), creation_time(creation_time) {
        pathes.emplace_back(path);
    }

    const std::filesystem::path &Image::getPath() const {
        return pathes.at(0);
    }


    const std::string &Image::getSha1Sum() const {
        return sha1_sum;
    }

    ImageType Image::getType() const {
        return type;
    }

    void Image::add_path(const std::filesystem::path &new_path) {
        pathes.emplace_back(new_path);
    }

    std::string Image::getFilename() const {
        auto a = getPath().filename();
        return getPath().filename().string();
    }

    void Image::setCreationTime(time_t creationTime) {
        creation_time = creationTime;
    }

    time_t Image::getCreationTime() const {
        return creation_time;
    }

    void Image::add_tag(const Tag &tag) {
        tag_ids.insert(tag.getIdentifier());
    }

    void Image::clear_tags() {
        tag_ids.clear();
    }

    const std::set<std::string> &Image::getTagIds() const {
        return tag_ids;
    }

    Image::Image() : pathes({""}), sha1_sum(""), type(ImageType::None), creation_time(0) {
    }

    QJsonObject Image::to_json() const {
        QJsonObject root{};
        QJsonArray pathes_arr{};
        QJsonArray tags_arr{};
        for (const auto & path : pathes) {
            pathes_arr.push_back(QString::fromStdString(path.string()));
        }
        for (const auto & tag_id : tag_ids) {
            tags_arr.push_back(QString::fromStdString(tag_id));
        }
        root[IMAGE_CHECKSUM_KEY] = QString::fromStdString(sha1_sum);
        root[IMAGE_PATHES_KEY] = pathes_arr;
        root[IMAGE_TAGS_KEY] = tags_arr;
        root[IMAGE_CREATION_KEY] = QJsonValue{static_cast<int>(creation_time)};
        root[IMAGE_TYPE_KEY] = QJsonValue{static_cast<int>(type)};
        return root;
    }

    Image Image::from_json(const QJsonObject & json) {
        std::vector<std::string> path_strings;
        std::set<std::string> tag_ids;
        for(auto const & path_string : json[IMAGE_PATHES_KEY].toArray()) {
            path_strings.emplace_back(path_string.toString().toStdString());
        }
        for(auto const & tag_id : json[IMAGE_TAGS_KEY].toArray()) {
            tag_ids.insert(tag_id.toString().toStdString());
        }

        return Image{
            path_strings,
            json[IMAGE_CHECKSUM_KEY].toString().toStdString(),
            static_cast<ImageType>(json[IMAGE_TYPE_KEY].toInt()),
            json[IMAGE_CREATION_KEY].toInt(),
            tag_ids
        };
    }

    Image::Image(const std::vector<std::string> &pathes,
                 std::string  sha1Sum,
                 ImageType type,
                 time_t creation_time,
                 const std::set<std::string>& tag_ids) :
    sha1_sum(std::move(sha1Sum)), tag_ids(tag_ids), type(type), creation_time(creation_time) {
        for (auto const & path_string : pathes) {
            this->pathes.emplace_back(path_string);
        }
    }

    bool Image::operator==(const Image &rhs) const {
        //not comparing pathes because they cannot change in the model anyway

        return tag_ids == rhs.tag_ids && sha1_sum == rhs.sha1_sum && type == rhs.type
        && creation_time == rhs.creation_time;
    }

}