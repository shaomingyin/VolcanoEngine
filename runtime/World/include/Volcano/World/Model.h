#ifndef VOLCANO_WORLD_MODEL_H
#define VOLCANO_WORLD_MODEL_H

#include <filesystem>
#include <map>
#include <utility>

#include <Volcano/World/Common.h>
#include <Volcano/World/Material.h>

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

    const std::map<std::filesystem::path, Material>& materials() const noexcept {
        return materials_;
    }

    Material& material(const std::filesystem::path& path) {
        return materials_[path];
    }

    const Material* findMaterial(const std::filesystem::path& path) const noexcept {
        const auto it = materials_.find(path);
        return it != materials_.end() ? &it->second : nullptr;
    }

    void setMaterial(std::filesystem::path path, Material material) {
        materials_.insert_or_assign(std::move(path), std::move(material));
    }

    void removeMaterial(const std::filesystem::path& path) {
        materials_.erase(path);
    }

    void clearMaterials() noexcept {
        materials_.clear();
    }

private:
    std::filesystem::path source_;
    std::map<std::filesystem::path, Material> materials_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MODEL_H
