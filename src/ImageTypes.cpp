#include "ImageTypes.h"

#include <regex>
#include <QString>
namespace possum {
    using namespace std;

    ImageType parse_extension(const string &file_path) {
        regex r("\\.(?:(jpe?g)|(png)|(bmp)|([A-Za-z]+))$", regex::icase);
        smatch match{};
        if (!regex_search(file_path, match, r)) {
            return ImageType::None;
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

} // namespace possum