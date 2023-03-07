//
// Created by felix on 3/6/23.
//
#include <gtest/gtest.h>
#include <filesystem>
#include <utility>
#include "imagesListModel.h"
using namespace std::filesystem;
using namespace std;
std::string TEST_DIR_PATH("/home/felix/Projects/picture_possum/test/test_dirs/dir1");
TEST(TestExifSimple, TestExif) {
    vector<string> targets = {"","","","2016:07:19 10:20:08"};
    vector<string> test_returns;
    for (const auto &dir_entry : directory_iterator(TEST_DIR_PATH)) {
        long file_size = dir_entry.file_size();
        unique_ptr<char[]> buffer = make_unique<char[]>(file_size);
        ifstream file_stream(dir_entry.path(), ios::binary | ios::in);
        file_stream.read(buffer.get(), static_cast<long>(file_size));
        test_returns.emplace_back(possum::ImagesListModel::get_exif_date(file_stream));
    }
    for (size_t i; i< targets.size(); i++) {
        ASSERT_EQ(targets.at(i), test_returns.at(i));
    }
}
