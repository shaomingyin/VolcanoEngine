//
//
#ifndef VOLANO_GRAPHICS_CANVAS_HPP
#define VOLANO_GRAPHICS_CANVAS_HPP

#include <SFML/Graphics/RenderTexture.hpp>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Renderable.hpp>

VOLCANO_GRAPHICS_BEGIN

class Canvas: public Renderable {
public:
    Canvas(Context& context);
    virtual ~Canvas() = default;

public:
    void render() override;

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates status) = 0;

private:
    sf::RenderTexture render_texture_;
};

VOLCANO_GRAPHICS_END

#endif // VOLANO_GRAPHICS_CANVAS_HPP
