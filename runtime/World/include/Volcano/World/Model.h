#ifndef VOLCANO_WORLD_MODEL_H
#define VOLCANO_WORLD_MODEL_H

#include <filesystem>
#include <map>
#include <string>
#include <utility>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Model final {
public:
    Model() = default;

    explicit Model(std::filesystem::path source)
        : source_(std::move(source)) {
    }

    constexpr const std::filesystem::path& source() const noexcept {
        return source_;
    }

    void setSource(std::filesystem::path value) {
        source_ = std::move(value);
    }

    const std::map<std::filesystem::path, std::string>& textureOverrides() const noexcept {
        return overrides_;
    }
    void setTextureOverride(std::filesystem::path material, std::string texture) {
        overrides_.insert_or_assign(std::move(material), std::move(texture));
    }

private:
    std::filesystem::path source_;
    std::map<std::filesystem::path, std::string> overrides_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MODEL_H
