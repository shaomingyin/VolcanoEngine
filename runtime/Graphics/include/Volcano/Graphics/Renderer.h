//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <entt/entt.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <Volcano/Math.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Frame.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(const entt::registry& registry);
    virtual ~Renderer() = default;

public:
    void reset() noexcept;
    void build(entt::entity camera_ent) noexcept;
    void render(const sf::RenderTarget& target) const noexcept;

private:
    const entt::registry& registry_;
    Frame frames_[2];
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
