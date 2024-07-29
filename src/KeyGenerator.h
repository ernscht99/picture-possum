#ifndef PICTURE_POSSUM_KEYGENERATOR_H
#define PICTURE_POSSUM_KEYGENERATOR_H

#include <string>
#include <memory>

namespace possum {
    ///This is a list of algorithms to generate keys for duplicate detection (see imagesListModel.h)
    enum KeyAlgorithm : uint8_t {
        HashWholeFile,
    };

    ///Generate a key from the contents of a file that has been loaded into memory, according to the algorithm
    ///argument
    std::string generate_key(const std::unique_ptr<char[]> &buffer, size_t length, KeyAlgorithm algorithm);
} //namespace possum

#endif //PICTURE_POSSUM_KEYGENERATOR_H
