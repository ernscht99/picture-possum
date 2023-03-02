#include <gtest/gtest.h>
#include <image.h>
#include <vector>

using namespace possum;
using namespace std;
TEST(TestLoadFiles, TestTypes) {
    vector<Image> images{Image::get_images("/home/felix/Projects/picture_possum/test/test_dirs/dir1", {Image::Type::JPEG})};

}