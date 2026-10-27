#ifndef VOLCANO_SCHEMA_FORMAT_H
#define VOLCANO_SCHEMA_FORMAT_H

#include <bit>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

#include <Volcano/ByteArray.h>
#include <Volcano/Schema/Common.h>

VOLCANO_SCHEMA_BEGIN
namespace detail {

using Json = nlohmann::json;

inline std::size_t checkedProduct(std::size_t left, std::size_t right) {
    if (right != 0 && left > std::numeric_limits<std::size_t>::max() / right) {
        throw std::invalid_argument("Resource size exceeds addressable range");
    }

    return left * right;
}

inline std::size_t checkedSum(std::size_t left, std::size_t right) {
    if (left > std::numeric_limits<std::size_t>::max() - right) {
        throw std::invalid_argument("Resource size exceeds addressable range");
    }

    return left + right;
}

inline std::string pathString(const std::filesystem::path& path) {
    const auto utf8 = path.generic_u8string();
    return std::string(utf8.begin(), utf8.end());
}

inline void validatePath(const std::filesystem::path& path, bool required) {
    const auto text = pathString(path);
    if (text.empty() && !required) {
        return;
    }
    if (text.empty() || text.front() == '/' || text.find_first_of("\\:") != std::string::npos ||
        text.find('\0') != std::string::npos) {
        throw std::invalid_argument("Resource path must be a relative PhysFS path");
    }

    std::size_t start = 0;
    while (start <= text.size()) {
        const auto end = text.find('/', start);
        const auto component = std::string_view(text).substr(start,
            end == std::string::npos ? text.size() - start : end - start);
        if (component.empty() || component == "." || component == "..") {
            throw std::invalid_argument("Resource path must not contain empty, '.' or '..' components");
        }
        if (end == std::string::npos) {
            break;
        }

        start = end + 1;
    }

    try {
        // JSON strings use the same UTF-8 encoding as resource paths.
        static_cast<void>(Json(text).dump());
    } catch (const Json::exception& error) {
        throw std::invalid_argument(std::string("Invalid UTF-8 resource path: ") + error.what());
    }
}

inline std::filesystem::path readPath(const Json& value) {
    const auto text = value.get<std::string>();
    return std::filesystem::path(std::u8string(text.begin(), text.end()));
}

inline void requireJsonHeader(const Json& value, std::string_view type) {
    if (!value.is_object() || value.at("type") != type ||
        !value.at("version").is_number_integer() || value.at("version") != 1) {
        throw std::invalid_argument("Unsupported resource JSON type or version");
    }
}

inline std::span<const std::uint8_t> take(std::span<const std::uint8_t>& input, std::size_t count) {
    if (count > input.size()) {
        throw std::invalid_argument("Truncated binary resource");
    }

    const auto result = input.first(count);
    input = input.subspan(count);
    return result;
}

inline std::uint32_t readU32(std::span<const std::uint8_t>& input) {
    const auto bytes = take(input, 4);
    return static_cast<std::uint32_t>(bytes[0]) |
        (static_cast<std::uint32_t>(bytes[1]) << 8) |
        (static_cast<std::uint32_t>(bytes[2]) << 16) |
        (static_cast<std::uint32_t>(bytes[3]) << 24);
}

inline void appendU32(ByteArray& output, std::uint32_t value) {
    for (unsigned shift = 0; shift < 32; shift += 8) {
        output.push_back(static_cast<std::uint8_t>(value >> shift));
    }
}

inline float readFloat(std::span<const std::uint8_t>& input) {
    static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559);
    return std::bit_cast<float>(readU32(input));
}

inline void appendFloat(ByteArray& output, float value) {
    static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559);
    appendU32(output, std::bit_cast<std::uint32_t>(value));
}

inline void requireBinaryHeader(std::span<const std::uint8_t>& input, std::string_view magic) {
    const auto bytes = take(input, 4);
    if (std::string_view(reinterpret_cast<const char*>(bytes.data()), bytes.size()) != magic ||
        readU32(input) != 1) {
        throw std::invalid_argument("Unsupported binary resource type or version");
    }
}

} // namespace detail
VOLCANO_SCHEMA_END

#endif // VOLCANO_SCHEMA_FORMAT_H
