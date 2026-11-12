#ifndef VOLCANO_ASSET_FORMAT_H
#define VOLCANO_ASSET_FORMAT_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <stdexcept>

#include <google/protobuf/io/zero_copy_stream.h>

#include <Volcano/Asset/Common.h>

VOLCANO_ASSET_BEGIN

class Format final {
public:
    Format() = delete;

    static std::size_t checkedProduct(std::size_t left, std::size_t right) {
        if (right != 0 && left > std::numeric_limits<std::size_t>::max() / right) {
            throw std::invalid_argument("Resource size exceeds addressable range");
        }
        return left * right;
    }

    static std::size_t checkedSum(std::size_t left, std::size_t right) {
        if (left > std::numeric_limits<std::size_t>::max() - right) {
            throw std::invalid_argument("Resource size exceeds addressable range");
        }
        return left + right;
    }

    static bool readExactBytes(google::protobuf::io::ZeroCopyInputStream& stream, void* dest, int count) {
        auto* out = static_cast<uint8_t*>(dest);
        while (count > 0) {
            const void* chunk = nullptr;
            int chunkSize = 0;
            if (!stream.Next(&chunk, &chunkSize)) {
                return false;
            }
            const int toCopy = std::min(count, chunkSize);
            std::memcpy(out, chunk, toCopy);
            out += toCopy;
            count -= toCopy;
            if (toCopy < chunkSize) {
                stream.BackUp(chunkSize - toCopy);
            }
        }
        return true;
    }
};

VOLCANO_ASSET_END

#endif // VOLCANO_ASSET_FORMAT_H
