//
//
#include <string_view>

#include <format>

#include <nlohmann_json.h>

namespace nlohmann {
    json loadFromStorage(SDL_Storage* storage, const char* path) {
        Uint64 size;
        if (!SDL_GetStorageFileSize(storage, path, &size)) {
            throw std::runtime_error("Failed to get user config size.");
        }
        if (size < 1) {
            throw std::runtime_error(std::format("Invalid json data {}.", path));
        }
        std::vector<uint8_t> data(size);
        if (!SDL_ReadStorageFile(storage, path, data.data(), size)) {
            throw std::runtime_error(std::format("Failed to load json from {}.", path));
        }
        return json::parse(data);
    }

    json findByPath(const nlohmann::json& j, const std::string& json_path) {
        size_t start = 0;
        json current = j;
        while (start < json_path.size()) {
            size_t end = json_path.find('.', start);
            if (end == std::string_view::npos) {
                throw std::runtime_error(std::format("Key {} not found.", json_path));
                break;
            }
            auto it = j.find(std::string_view(json_path.data() + start, end - start));
            if (it != j.end()) {
                current = *it;
            } else {
                throw std::runtime_error(std::format("Key {} not found.", json_path));
            }
            start = end + 1;
        }
        return current;
    }
}
