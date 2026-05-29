//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <atomic>

#include <entt/entt.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <Volcano/Math.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Frame.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(World::Scene& scene);
    virtual ~Renderer();

public:
    void build(entt::entity camera_ent = entt::null) noexcept;
    void draw(const sf::RenderTarget& target) const noexcept;

private:
    void buildCamera(entt::entity ent) noexcept;
    void buildLoading() noexcept;
    void buildPlaying() noexcept;
    void buildError() noexcept;
    void onModelAdded(entt::entity ent) noexcept;
    void onModelRemoved(entt::entity ent) noexcept;

private:
    World::Scene& scene_;
    Frame frame_;
    std::atomic_int current_frame_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
