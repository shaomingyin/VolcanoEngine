#ifndef VOLCANO_WORLD_MODEL_H
#define VOLCANO_WORLD_MODEL_H

#include <cstddef>
#include <filesystem>
#include <utility>
#include <vector>

#include <Volcano/World/Common.h>
#include <Volcano/World/Material.h>

VOLCANO_WORLD_BEGIN

class Model final {
public:
    Model() = default;
    ~Model() = default;
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    Model(Model&&) noexcept = default;
    Model& operator=(Model&&) noexcept = default;

    explicit Model(std::filesystem::path source)
        : source_(std::move(source)) {
    }

    constexpr const std::filesystem::path& source() const noexcept {
        return source_;
    }

    void setSource(std::filesystem::path value) {
        source_ = std::move(value);
    }

    const std::vector<Material>& materials() const noexcept {
        return materials_;
    }

    Material& material(std::size_t part) {
        if (part >= materials_.size()) {
            materials_.resize(part + 1);
        }
        return materials_[part];
    }

    const Material* findMaterial(std::size_t part) const noexcept {
        return part < materials_.size() ? &materials_[part] : nullptr;
    }

    void setMaterial(std::size_t part, Material material) {
        if (part >= materials_.size()) {
            materials_.resize(part + 1);
        }
        materials_[part] = std::move(material);
    }

    void removeMaterial(std::size_t part) noexcept {
        if (part >= materials_.size()) {
            return;
        }
        materials_[part] = Material{};
        while (!materials_.empty() && materials_.back().textures().empty()) {
            materials_.pop_back();
        }
    }

    void clearMaterials() noexcept {
        materials_.clear();
    }

private:
    std::filesystem::path source_;
    std::vector<Material> materials_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MODEL_H
