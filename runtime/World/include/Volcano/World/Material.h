#ifndef VOLCANO_WORLD_MATERIAL_H
#define VOLCANO_WORLD_MATERIAL_H

#include <cstdint>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>

#include <Volcano/World/Common.h>
#include <Volcano/World/Texture.h>

VOLCANO_WORLD_BEGIN

class Material final {
public:
    enum class Slot : std::uint8_t {
        Albedo,
    };

public:
    Material() = default;
    ~Material() = default;
    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;
    Material(Material&&) noexcept = default;
    Material& operator=(Material&&) noexcept = default;

    const std::map<Slot, std::unique_ptr<Texture>>& textures() const noexcept {
        return textures_;
    }

    void setTexture(Slot slot, std::unique_ptr<Texture> source) {
        if (!source) {
            throw std::invalid_argument("Material texture cannot be null");
        }
        textures_.insert_or_assign(slot, std::move(source));
    }

    void removeTexture(Slot slot) {
        textures_.erase(slot);
    }

    void clearTextures() noexcept {
        textures_.clear();
    }

    std::unique_ptr<Texture> takeTexture(Slot slot) noexcept {
        const auto it = textures_.find(slot);
        if (it == textures_.end()) {
            return nullptr;
        }
        auto result = std::move(it->second);
        textures_.erase(it);
        return result;
    }

    const Texture* texture(Slot slot) const noexcept {
        const auto it = textures_.find(slot);
        return it != textures_.end() ? it->second.get() : nullptr;
    }

    Texture* texture(Slot slot) noexcept {
        const auto it = textures_.find(slot);
        return it != textures_.end() ? it->second.get() : nullptr;
    }

    bool hasTexture(Slot slot) const noexcept {
        return textures_.contains(slot);
    }

private:
    std::map<Slot, std::unique_ptr<Texture>> textures_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MATERIAL_H
