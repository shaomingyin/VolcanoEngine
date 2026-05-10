//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <Volcano/Math.h>
#include <Volcano/Stage/Map.h>
#include <Volcano/Stage/Camera.h>
#include <Volcano/Stage/Lighting.h>
//#include <Volcano/Stage/Model.h>
#include <Volcano/Stage/Screen.h>
#include <Volcano/Stage/Scene.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(Stage::Scene& scene);
    virtual ~Renderer() = default;

public:
    virtual void reset() noexcept;
    virtual void build(entt::entity camera_ent) noexcept;
    virtual void render() noexcept;

protected:

private:
    Stage::Scene& scene_;
    Eigen::Matrix4f projection_matrix_;
    Eigen::Affine3f view_matrix_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
