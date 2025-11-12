//
//
#include <format>
#include <filesystem>
#include <stdexcept>

#include <Volcano/Framework/JsonUtils.h>

VOLCANO_FRAMEWORK_BEGIN

namespace JsonUtils {
    std::vector<uint8_t> readPhysFile(const char* filepath) {
        VOLCANO_ASSERT(filepath != nullptr);
        auto fp = PHYSFS_openRead(filepath);
        if (fp == nullptr) {
            throw std::runtime_error(std::format("Failed to read phys file: {}.", filepath));
        }
        auto size = PHYSFS_fileLength(fp);
        std::vector<uint8_t> data;
        if (size > 0) {
            data.resize(size);
            auto read_result = PHYSFS_readBytes(fp, data.data(), size);
            if (read_result != size) {
                throw std::runtime_error(std::format("Failed to read phys file: {}", filepath));
            }
        }
        return data;
    }

    nlohmann::json find(const nlohmann::json& json, const char* jsonpath) {
        VOLCANO_ASSERT(jsonpath != nullptr);
        VOLCANO_ASSERT(jsonpath[0] == '/');
        auto path = std::filesystem::path(jsonpath).lexically_normal();
        const nlohmann::json* p = &json;
        for (auto& node: path) {
            p = &p->at(node.string());
        }
        return *p;
    }
}

VOLCANO_FRAMEWORK_END
