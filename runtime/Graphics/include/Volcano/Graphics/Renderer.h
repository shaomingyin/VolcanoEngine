//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <Volcano/Math.h>
#include <Volcano/World/Map.h>
#include <Volcano/World/Camera.h>
#include <Volcano/World/Lighting.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/Screen.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(entt::registry& scene);
    virtual ~Renderer() = default;

public:
    virtual void reset() noexcept;
    virtual void build(entt::entity camera_ent) noexcept;
    virtual void render() noexcept;

protected:

private:
    entt::registry& scene_;
    Eigen::Matrix4f projection_matrix_;
    Eigen::Affine3f view_matrix_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
