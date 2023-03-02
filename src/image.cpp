//
// Created by felix on 2/19/23.
//

#include "image.h"

#include <utility>
namespace {
    std::vector<unsigned char> get_sha1(const std::unique_ptr<char[]>& buffer, size_t length) {
        unsigned char digest[20];
        SHA1(reinterpret_cast<const unsigned char *>(buffer.get()), length, digest);
        return std::vector<unsigned char>{std::begin(digest), std::end(digest)};
    }
}

namespace possum {
    using namespace std;
    using namespace std::filesystem;

    std::vector<Image> Image::get_images(const string &directory_path, const set<Image::Type> &valid_types) {
        vector<Image> image_vector;
        for (const auto &dir_entry : directory_iterator(directory_path)){
            Image::Type file_type{parse_extension(dir_entry.path())};

            ///For each file in directory, see if it is an image file we want (valid type)
            if (dir_entry.is_regular_file() && valid_types.contains(file_type)){
                ///Read file into memory to calculate hash
                size_t file_size = dir_entry.file_size();
                unique_ptr<char[]> buffer = make_unique<char[]>(file_size);
                ifstream file_stream(dir_entry.path(), ios::binary | ios::in);
                file_stream.read(buffer.get(), static_cast<long>(file_size));

                ///Calculate hash and push Image object to vector that is to be returned
                image_vector.emplace_back(dir_entry.path(), get_sha1(buffer, file_size), file_type);
            }
        }
        return image_vector;
    }

    Image::Image(string path, const vector<unsigned char> &sha1Sum, Type type) :
    path(std::move(path)), sha1_sum(sha1Sum), type(type) {}

    Image::Type Image::parse_extension(const string &file_path) {
        regex r("\\.(?:(jpe?g)|(png)|(bmp)|([A-Za-z]+))$", std::regex::icase);
        smatch match{};
        if (!regex_search(file_path, match, r)) {
            return Image::Type::None;
        }
        size_t match_index = 1;
        for (; !match[match_index].matched; ++match_index);
        switch (match_index) {
            case 1:
                return JPEG;
            case 2:
                return PNG;
            case 3:
                return BMP;
            default:
                return Other;
        }
    }

    const string &Image::getPath() const {
        return path;
    }

    const vector<unsigned char> &Image::getSha1Sum() const {
        return sha1_sum;
    }

    Image::Type Image::getType() const {
        return type;
    }
}