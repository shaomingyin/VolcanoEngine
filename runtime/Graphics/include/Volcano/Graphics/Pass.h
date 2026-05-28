//
//
#ifndef VOLCANO_GRAPHICS_PASS_H
#define VOLCANO_GRAPHICS_PASS_H

#include <SFML/Graphics/RenderTarget.hpp>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Pass {
public:
	Pass();
	virtual ~Pass() = default;

public:
	//virtual void draw(const sf::RenderTarget& target) const noexcept = 0;

private:
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PASS_H
