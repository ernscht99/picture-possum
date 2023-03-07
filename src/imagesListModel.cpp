//
// Created by felix on 3/2/23.
//

#include "imagesListModel.h"

namespace {
    enum ByteOrder{
        M, I
    };
    uint16_t parse2(unsigned char* data, ByteOrder order){
        if (order == M)
            return (data[0] << 8) + data[1];
        return (data[1] << 8) + data[0];
    }
    uint32_t parse4(unsigned char* data, ByteOrder order){
        if (order == M)
            return (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
        return (data[3] << 24) + (data[2] << 16) + (data[1] << 8) + data[0];
    }
    std::string get_sha1(const std::unique_ptr<char[]>& buffer, size_t length) {
        unsigned char digest[20];
        SHA1(reinterpret_cast<const unsigned char *>(buffer.get()), length, digest);
        std::ostringstream readable;
        readable << std::hex;
        for (unsigned char c : digest){
            readable << static_cast<int>(c);
        }
        return readable.str();
    }

   std::chrono::year_month_day estimate_date(const std::string& filename, const std::unique_ptr<char[]>& buffer, size_t length) {

    }
}

namespace possum{
    using namespace std;
    using namespace std::filesystem;

    void ImagesListModel::load_images(const string &directory_path, const Settings &settings) {
        for (const auto &dir_entry : directory_iterator(directory_path)){
            ImageType file_type{parse_extension(dir_entry.path())};

            ///For each file in directory, see if it is an image file we want (valid type)
            if (dir_entry.is_regular_file() && settings.valid_types.contains(file_type)){
                ///Read file into memory to calculate hash
                size_t file_size = dir_entry.file_size();
                unique_ptr<char[]> buffer = make_unique<char[]>(file_size);
                ifstream file_stream(dir_entry.path(), ios::binary | ios::in);
                file_stream.read(buffer.get(), static_cast<long>(file_size));
                get_exif_date(file_stream);
                ///Calculate hash and push Image object to vector that is to be returned
                time_t estimated_date = estimate_date(file_stream,settings,dir_entry.path().filename().string());
                insert_image({dir_entry.path(), get_sha1(buffer, file_size), file_type, estimated_date});
            }
        }
        dataChanged(createIndex(0,0), createIndex(static_cast<int>(hash_handles.size()),0));
    }

    QVariant ImagesListModel::data(const QModelIndex &index, int role) const {
        if (!index.isValid())
            return {};

        if (static_cast<unsigned long>(index.row()) >= hash_handles.size())
            return {};

        if (role == Qt::DisplayRole)
            return {QString::fromStdString(hash_handles.at(index.row()))};
        else
            return {};
    }

    QVariant ImagesListModel::headerData(int section, Qt::Orientation orientation, int role) const {
        return {};
    }

    int ImagesListModel::rowCount(const QModelIndex &parent) const {
        return static_cast<int>(hash_handles.size());
    }

    ImagesListModel::ImagesListModel(QObject *parent) : QAbstractListModel(parent) {
    }


    void ImagesListModel::insert_image(const Image& inserting_image) {
        auto image_iter ( image_map.find(inserting_image.getSha1Sum()));
        if(image_iter == image_map.end()){
            image_map.insert({inserting_image.getSha1Sum(), std::make_unique<Image>(inserting_image)});
            hash_handles.emplace_back(inserting_image.getSha1Sum());
        } else {
            Image * existing_image = image_iter->second.get();
            existing_image->add_path(inserting_image.getPath());
            if (inserting_image.getCreationTime() < existing_image->getCreationTime())
                image_iter->second->setCreationTime(inserting_image.getCreationTime());
        }
    }

    Image ImagesListModel::get_image(const string &hash) {
        return *image_map.find(hash)->second;
    }


    string ImagesListModel::get_exif_date(ifstream &file_stream) {
        file_stream.seekg(0);
        TinyEXIF::EXIFInfo exif_data(file_stream);
        return exif_data.DateTimeOriginal;
    }

    time_t
    ImagesListModel::estimate_date(std::ifstream &file_stream, const Settings &settings, const std::string &file_name) {
        time_t epoch = 0;
        tm estimated_date = *::gmtime(&epoch);
        string exif_date = get_exif_date(file_stream);
        if (! exif_date.empty()) {
            strptime(exif_date.c_str(), "%Y:%m:%d %H:%M:%S", &estimated_date);
            return ::mktime(&estimated_date);
        }

        size_t first_digit = file_name.find_first_of("0123456789");
        if (first_digit == string::npos)
            first_digit = 0;

        for (const string& format : settings.date_conversion_formats) {
            if (strptime(file_name.c_str() + first_digit, format.c_str(), &estimated_date))
                return ::mktime(&estimated_date);
        }
        return epoch;
    }
}
