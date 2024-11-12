//
//
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

View::View(int w, int h)
	: hud_(w, h) {
}

View::~View() {
}

void View::reset() {
	directional_lights_.clear();
	point_lights_.clear();
	spot_lights_.clear();
	renderable_objects_.clear();
}

VOLCANO_GRAPHICS_END
