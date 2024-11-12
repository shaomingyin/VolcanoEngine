//
//
#ifndef VOLCANO_GRAPHICS_RENDERABLEOBJECT_H
#define VOLCANO_GRAPHICS_RENDERABLEOBJECT_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class RenderableObject {
public:
	RenderableObject() = default;
	virtual ~RenderableObject() = default;

public:
	virtual void render() = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERABLEOBJECT_H
