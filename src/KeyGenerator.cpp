#include "KeyGenerator.h"
#include <openssl/sha.h>
#include <sstream>

namespace {
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
}

namespace possum {
    std::string generate_key(const std::unique_ptr<char[]>& buffer, size_t length, KeyAlgorithm algorithm) {
        switch (algorithm) {
            case HashWholeFile:
            default:
                return get_sha1(buffer, length);
        }
    }
}
