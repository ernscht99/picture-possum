#include "KeyGenerator.h"
#include <sstream>

#if __has_include(<openssl/sha.h>)

#include <openssl/sha.h>

#else
#include "TinySHA1.hpp"
namespace {
    ///If openssl is unavailable, uses TinySHA1 as a backup (which is slower)
    unsigned char *SHA1(const unsigned char *data, size_t count, unsigned char *md_buf) {
        sha1::SHA1 s;
        s.processBytes(data, count);
        s.getDigestBytes(md_buf);
        return md_buf;
    }
}
#endif

namespace {
    ///return the hex string digest of the data in buffer
    std::string get_sha1(const std::unique_ptr<char[]> &buffer, size_t length) {
        unsigned char digest[20];
        SHA1(reinterpret_cast<const unsigned char *>(buffer.get()), length, digest);
        std::ostringstream readable;
        readable << std::hex;
        for (unsigned char c: digest) {
            readable << static_cast<int>(c);
        }
        return readable.str();
    }
}

namespace possum {
    std::string generate_key(const std::unique_ptr<char[]> &buffer, size_t length, KeyAlgorithm algorithm) {
        switch (algorithm) {
            case HashWholeFile:
            default:
                return get_sha1(buffer, length);
        }
    }
}
