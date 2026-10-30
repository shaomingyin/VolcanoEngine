//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <string>
#include <map>
#include <memory>
#include <stdexcept>

#include <entt/entt.hpp>

#include <Volcano/Version.h>
#include <Volcano/World/Common.h>
#include <Volcano/World/Texture.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
public:
    Scene();
    virtual ~Scene();

public:
    constexpr const std::string& name() const noexcept {
        return name_;
    }

    void setName(const std::string& v) {
        name_ = v;
    }

    constexpr const std::string& description() const noexcept {
        return description_;
    }

    void setDescription(const std::string& v) {
        description_ = v;
    }

    constexpr const VersionNumber& version() const noexcept {
        return version_;
    }

    void setVersion(const VersionNumber& v) {
        version_ = v;
    }

    void addTexture(std::string id, std::unique_ptr<Texture> texture) {
        if (id.empty() || !texture || textures_.contains(id)) {
            throw std::invalid_argument("Texture requires a unique nonempty ID and a description");
        }
        textures_.emplace(std::move(id), std::move(texture));
    }

    const std::map<std::string, std::unique_ptr<Texture>>& textures() const noexcept {
        return textures_;
    }
    Texture& texture(const std::string& id) {
        return *textures_.at(id);
    }
    const Texture& texture(const std::string& id) const {
        return *textures_.at(id);
    }

    void update(Clock::duration elapsed);
    virtual entt::entity player() const noexcept = 0;

protected:
    virtual void onUpdate(Clock::duration elapsed) = 0;

private:
    std::map<std::string, std::unique_ptr<Texture>> textures_;
    std::string name_;
    std::string description_;
    VersionNumber version_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
