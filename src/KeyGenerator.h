#ifndef PICTURE_POSSUM_KEYGENERATOR_H
#define PICTURE_POSSUM_KEYGENERATOR_H

#include <string>
#include <memory>

namespace possum {
    enum KeyAlgorithm {
        HashWholeFile,
    };

    std::string generate_key(const std::unique_ptr<char[]>& buffer, size_t length, KeyAlgorithm algorithm);
}


#endif //PICTURE_POSSUM_KEYGENERATOR_H
