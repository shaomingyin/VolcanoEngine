//
//
#ifndef VOLANO_GRAPHICS_RENDERABLE_HPP
#define VOLANO_GRAPHICS_RENDERABLE_HPP

#include <Volcano/Game/Component.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Context.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderable {
public:
    Renderable(Context& context);
    virtual ~Renderable() = default;

public:
    virtual void render() = 0;

private:
    Context& context_;
};

VOLCANO_GRAPHICS_END

#endif // VOLANO_GRAPHICS_RENDERABLE_HPP
