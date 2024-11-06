//
//
#ifndef VOLCANO_GRAPHICS_CAMERA_H
#define VOLCANO_GRAPHICS_CAMERA_H

#include <Volcano/World/Component.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Camera: public World::Movable {
public:
	Camera();
	virtual ~Camera();
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERA_H
