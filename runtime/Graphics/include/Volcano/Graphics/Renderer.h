//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <rttr/type>

#include <Volcano/Math.h>
#include <Volcano/World/Map.h>
#include <Volcano/World/Camera.h>
#include <Volcano/World/Lighting.h>
//#include <Volcano/World/Model.h>
#include <Volcano/World/Screen.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Context.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Pass.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer: public Context {
    RTTR_ENABLE()

public:
    Renderer();
    virtual ~Renderer() = default;

public:
    void reset() noexcept;
    void build(const World::Scene& scene, entt::entity camera_ent) noexcept;
    void render(Target& target) noexcept;

protected:
	virtual void draw(const View& view, Target& target) noexcept = 0;

private:
    View views_[2];
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
