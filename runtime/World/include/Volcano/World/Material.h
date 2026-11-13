#ifndef VOLCANO_WORLD_MATERIAL_H
#define VOLCANO_WORLD_MATERIAL_H

#include <cstdint>
#include <map>
#include <utility>

#include <entt/entt.hpp>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Material final {
public:
    enum class Slot : std::uint8_t {
        Albedo,
    };

public:
    Material() = default;

    const std::map<Slot, entt::entity>& textures() const noexcept {
        return textures_;
    }

    void setTexture(Slot slot, entt::entity source) {
        textures_.insert_or_assign(slot, source);
    }

    void removeTexture(Slot slot) {
        textures_.erase(slot);
    }

    void clearTextures() noexcept {
        textures_.clear();
    }

    entt::entity texture(Slot slot) const noexcept {
        const auto it = textures_.find(slot);
        return it != textures_.end() ? it->second : entt::null;
    }

    bool hasTexture(Slot slot) const noexcept {
        return textures_.contains(slot);
    }

private:
    std::map<Slot, entt::entity> textures_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MATERIAL_H
