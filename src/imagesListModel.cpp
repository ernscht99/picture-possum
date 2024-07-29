#include <QMessageBox>
#include <QProgressDialog>
#include <thread>
#include <mutex>
#include <iostream>
#include "imagesListModel.h"

namespace {
    QString from_timestamp(time_t timestamp) {
        char time_string_buffer[64];
        tm time{};
        ::localtime_r(&timestamp, &time);
        ::strftime(time_string_buffer, 64,"%Y-%m-%d", &time);
        return QString{time_string_buffer};
    }
    std::filesystem::path get_subdir_path(const std::filesystem::path& parent,const std::string_view & subdir) {
        std::filesystem::path subdir_path{parent};
        subdir_path.append(subdir);
        return subdir_path;
    }

    std::filesystem::path get_subdir_path(const std::filesystem::path& parent, const std::string& subdir) {
        return get_subdir_path(parent, std::string_view{subdir});
    }

    void create_subdir_if_not_exist(const std::filesystem::path& path) {
        if(std::filesystem::exists(path) && std::filesystem::is_directory(path))
            return;
        std::filesystem::create_directory(path);
    }

    void symlink(const std::filesystem::path& path_dir, const std::filesystem::path& path_file) {
        std::filesystem::path path_dir_copy{path_dir};
        path_dir_copy.append(path_file.filename().c_str());
        std::filesystem::create_symlink(path_file, path_dir_copy);
    }
}

namespace possum{
    using namespace std;
    using namespace std::filesystem;

    void ImagesListModel::load_images(const std::string &directory_path) {

        QProgressDialog progress{};
        progress.setCancelButton(nullptr);
        progress.setLabelText("Detecting Duplicates");
        progress.setWindowModality(Qt::WindowModal);
        progress.setMaximum(static_cast<int>(std::distance(directory_iterator(directory_path), directory_iterator{})));
        progress.show();

        int count = 0;
        std::vector<std::thread> threadPool;
        auto it = directory_iterator(directory_path);
        std::mutex it_mutex;
        for (size_t i = 0; i < DUPLICATION_DETECTION_THREADS; ++i){
            threadPool.emplace_back([this, &it_mutex, &it, &count]() {
                while(true) {
                    std::unique_lock it_lock(it_mutex);
                    if(it == end(it))
                        return;
                    const auto& dir_entry = *it++;
                    count++;
                    it_lock.unlock();
                    ImageType file_type{parse_extension(dir_entry.path())};

                    ///For each file in directory, see if it is an image file we want (valid type)
                    if (dir_entry.is_regular_file() && settings.valid_types.contains(file_type)) {
                        ///Read file into memory
                        size_t file_size = dir_entry.file_size();
                        unique_ptr<char[]> buffer = make_unique<char[]>(file_size);
                        ifstream file_stream(dir_entry.path(), ios::binary | ios::in);
                        file_stream.read(buffer.get(), static_cast<long>(file_size));
                        get_exif_date(file_stream);
                        ///Calculate key and push Image object to vector that is to be returned
                        time_t estimated_date = estimate_date(file_stream, dir_entry.path().filename().string());

                        it_lock.lock();
                        insert_image({dir_entry.path(), generate_key(buffer, file_size, HashWholeFile), file_type,
                                      estimated_date});
                    }
                }
            });
        }
        while(it != end(it)){ //periodically get the current progress while the threads are running
            progress.setValue(count);
            std::this_thread::sleep_for(chrono::milliseconds(200));
        }

        for (auto& t :threadPool) {
            t.join();
        }
        /*
        for (const auto &dir_entry : directory_iterator(directory_path)){
            ImageType file_type{parse_extension(dir_entry.path())};

            ///For each file in directory, see if it is an image file we want (valid type)
            if (dir_entry.is_regular_file() && settings.valid_types.contains(file_type)){
                ///Read file into memory
                size_t file_size = dir_entry.file_size();
                unique_ptr<char[]> buffer = make_unique<char[]>(file_size);
                ifstream file_stream(dir_entry.path(), ios::binary | ios::in);
                file_stream.read(buffer.get(), static_cast<long>(file_size));
                get_exif_date(file_stream);
                ///Calculate key and push Image object to vector that is to be returned
                time_t estimated_date = estimate_date(file_stream, dir_entry.path().filename().string());
                insert_image({dir_entry.path(), generate_key(buffer, file_size, HashWholeFile), file_type, estimated_date});
            }
            progress.setValue(count++);
        }
         */
        emit layoutChanged();
    }

    QVariant ImagesListModel::data(const QModelIndex &index, int role) const {
        if (!index.isValid())
            return {};
        if (role == DataView)
            return {QString::fromStdString(hash_handles.at(index.row()))};
        if (role == Qt::FontRole) {
            if (index.column() == 2)
                return {QFont{"Emoji"}};
        }
        if (role == Qt::DisplayRole) {
            if (index.column() == 0) {
                std::string file_name{image_map.find(hash_handles.at(index.row()))->second->getFilename()};
                return {QString::fromStdString(file_name)};
            }
            if (index.column() == 1) {
                time_t timestamp = image_map.find(hash_handles.at(index.row()))->second->getCreationTime();
                return timestamp ? from_timestamp(timestamp) : QString{"No date"};
            }
            if (index.column() == 2) {
                auto tag_ids = image_map.find(hash_handles.at(index.row()))->second->getTagIds();
                stringstream builder;
                for (const auto & tag_id : tag_ids){
                    builder << settings.render_tag_symbol(tag_id);
                    builder << " ";
                }
                return QString::fromStdString(builder.str());
            }
        }
        return {};
    }

    QVariant ImagesListModel::headerData(int section, Qt::Orientation orientation, int role) const {
        if (role != Qt::DisplayRole)
            return {};
        if (orientation == Qt::Horizontal)
            switch (section){
            case 0:
                return { QString("File Name") };
            case 1:
                return {QString("Time")};
            case 2:
                return {QString("Tags")};
            default:
                return {};
            }
        return {};
    }

    int ImagesListModel::rowCount(const QModelIndex &) const {
        return static_cast<int>(hash_handles.size());
    }
    int ImagesListModel::columnCount(const QModelIndex &) const {
        return 3;
    }

    ImagesListModel::ImagesListModel(Settings settings, QObject *parent) : QAbstractTableModel(parent), settings(std::move(settings)), unsaved_changes(false) {
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
        auto to_get = image_map.find(hash);
        if (to_get == image_map.end())
            return Image{};
        return *to_get->second;
    }


    string ImagesListModel::get_exif_date(ifstream &file_stream) {
        file_stream.seekg(0);
        TinyEXIF::EXIFInfo exif_data(file_stream);
        return exif_data.DateTimeOriginal;
    }

    time_t
    ImagesListModel::estimate_date(std::ifstream &file_stream, const std::string &file_name) {
        time_t epoch = 0;

        ///Try to get date from exif first, if it is there.
        tm estimated_date = *::gmtime(&epoch);
        string exif_date = get_exif_date(file_stream);
        if (! exif_date.empty()) {
            strptime(exif_date.c_str(), "%Y:%m:%d %H:%M:%S", &estimated_date);
            return ::mktime(&estimated_date);
        }

        ///If there is no EXIF, try to extract from filename according to set format strings
        size_t first_digit = file_name.find_first_of("0123456789");
        if (first_digit == string::npos)
            first_digit = 0;

        for (const QString& format : settings.date_conversion_formats) {
            if (strptime(file_name.c_str() + first_digit, format.toStdString().c_str(), &estimated_date)) {
                epoch = ::mktime(&estimated_date);
                break;
            }
        }
        ///Little safeguard against dates after 2100
        if (epoch > 4109223911)
            return 0;

        return epoch;
    }

    void ImagesListModel::sort(int column, Qt::SortOrder order) {
        auto compare = [this, &order, &column](const string& a, const string& b) {
            auto first = this->image_map.find(a);
            auto second = this->image_map.find(b);
            if (order == Qt::DescendingOrder) {
                swap(first, second);
            }
            switch(column) {
                case 0:
                    return first->second->getFilename() < second->second->getFilename();
                case 1:
                default:
                    return first->second->getCreationTime() < second->second->getCreationTime();
            }
        };
        std::stable_sort(hash_handles.begin(), hash_handles.end(), compare);
        layoutChanged();
    }

    void ImagesListModel::update_image(const Image &updater) {
        auto to_be_updated = image_map.find(updater.getSha1Sum());
        if(to_be_updated == image_map.end())
            return;

        if(*to_be_updated->second != updater)
            unsaved_changes = true;
        *to_be_updated->second = updater;
        layoutChanged();
    }

    void ImagesListModel::setSettings(const Settings &settings) {
        ImagesListModel::settings = settings;
    }


    QJsonObject ImagesListModel::to_json() const {
        QJsonObject root{};
        QJsonArray image_array{};
        for (const auto & [k, image_ptr] : image_map) {
            image_array.push_back(image_ptr->to_json());
        }
        root[SETTINGS_KEY] = settings.to_json();
        root[IMAGE_LIST_KEY] =  image_array;
        return root;
    }

    ImagesListModel ImagesListModel::from_json(const QJsonObject & json) {
        std::vector<Image> images;
        std::map<std::string, std::unique_ptr<Image>> image_map;
        for (auto const & image_json : json[IMAGE_LIST_KEY].toArray()) {
            Image image = Image::from_json(image_json.toObject());
            image_map.insert({image.getSha1Sum(), make_unique<Image>(image)});
        }
        return ImagesListModel{
            Settings::from_json(json[SETTINGS_KEY].toObject()),
            std::move(image_map)
        };
    }


    ImagesListModel::ImagesListModel(Settings settings, std::map<std::string, std::unique_ptr<Image>> images,
                                     QObject *) : image_map(std::move(images)), settings(std::move(settings)), unsaved_changes(false){
        for (const auto & [key, ignore] : image_map) {
            hash_handles.emplace_back(key);
        }
    }

    bool ImagesListModel::save(const path &path) {
        QFile file {path};
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODeviceBase::Truncate))
            return false;
        file.write(QJsonDocument{this->to_json()}.toJson());
        file.close();
        unsaved_changes = false;
        return true;
    }

    void ImagesListModel::load(const path &path) {
        QFile file {path};
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        ImagesListModel retval = ImagesListModel::from_json(QJsonDocument::fromJson(file.readAll()).object());
        file.close();
        this->image_map = std::move(retval.image_map);
        this->hash_handles = std::move(retval.hash_handles);
        this->settings = retval.settings;
        layoutChanged();
    }

    const Settings &ImagesListModel::getSettings() const {
        return settings;
    }

    bool ImagesListModel::has_unsaved_changes() const {
        return unsaved_changes;
    }

    void ImagesListModel::generate_sorted_dir(const std::filesystem::path &path) {
        QProgressDialog progress{};
        progress.setCancelButton(nullptr);
        progress.setLabelText("Generating sorted directory structure");
        progress.setWindowModality(Qt::WindowModal);
        progress.setMaximum(static_cast<int>(image_map.size()));
        size_t i = 0;

        std::filesystem::path sorted_path = path;
        sorted_path.append("sorted");
        if(exists(sorted_path)) {
            QMessageBox msgBox;
            msgBox.setText("Directory 'sorted' already exists.");
            msgBox.setInformativeText("Do you want to override its contents?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            switch (ret) {
                case QMessageBox::No:
                    return;
                case QMessageBox::Yes:
                    std::filesystem::remove_all(sorted_path);
                    break;
                default:
                    return;
            }
            //ask if override
        }
        //Level 0
        std::filesystem::create_directory(sorted_path);

        //Level 1
        std::filesystem::path by_tag_path{sorted_path};
        by_tag_path.append("by_tag");
        std::filesystem::create_directory(by_tag_path);

        std::filesystem::path by_date_path{sorted_path};
        by_date_path.append("by_date");
        std::filesystem::create_directory(by_date_path);

        //Level 2
        std::filesystem::path no_tag_path{by_tag_path};
        no_tag_path.append("untagged");
        std::filesystem::create_directory(no_tag_path);

        std::filesystem::path no_date_path{by_date_path};
        no_date_path.append("undated");
        std::filesystem::create_directory(no_date_path);

        std::filesystem::path by_year_path{by_date_path};
        by_year_path.append("by_year");
        std::filesystem::create_directory(by_year_path);

        std::filesystem::path by_month_path{by_date_path};
        by_month_path.append("by_month");
        std::filesystem::create_directory(by_month_path);

        std::filesystem::path by_day_path{by_date_path};
        by_day_path.append("by_day");
        std::filesystem::create_directory(by_day_path);


        //generate symlinks
        for(auto & it : image_map) {
            auto& image = it.second;
            auto& image_path = image->getPath();

            //tags
            auto& tag_ids = image->getTagIds();
            if(tag_ids.empty()) {
                symlink(no_tag_path, image_path);
            } else {
                for(const auto& tag_id : tag_ids) {
                    auto tag_path = get_subdir_path(by_tag_path, settings.render_tag_name(tag_id));
                    create_subdir_if_not_exist(tag_path);
                    symlink(tag_path, image_path);
                }
            }

            //dates
            if(image->getCreationTime() == 0) {
                symlink(no_date_path,image_path);
            } else {
                std::string date_string = from_timestamp(image->getCreationTime()).toStdString();
                std::string_view year = std::string_view(date_string).substr(0, 4);
                std::string_view month = std::string_view(date_string).substr(0, 7);
                std::string_view day = std::string_view(date_string).substr(0, 10);

                //year
                auto year_path = get_subdir_path(by_year_path, year);
                create_subdir_if_not_exist(year_path);
                symlink(year_path, image_path);

                //month
                auto month_year_path = get_subdir_path(by_month_path, year);
                create_subdir_if_not_exist(month_year_path);
                auto month_path = get_subdir_path(month_year_path, month);
                create_subdir_if_not_exist(month_path);
                symlink(month_path, image_path);

                //day
                auto day_year_path = get_subdir_path(by_day_path, year);
                create_subdir_if_not_exist(day_year_path);
                auto day_month_path = get_subdir_path(day_year_path, month);
                create_subdir_if_not_exist(day_month_path);
                auto day_path = get_subdir_path(day_month_path, day);
                create_subdir_if_not_exist(day_path);
                symlink(day_path, image_path);
            }
            progress.setValue(static_cast<int>(i++));
        }
    }

    void ImagesListModel::clear() {
        this->hash_handles.clear();
        this->image_map.clear();
        this->unsaved_changes = false;
        layoutChanged();
    }

}
