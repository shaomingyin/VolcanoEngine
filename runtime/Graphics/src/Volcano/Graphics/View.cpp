//
//
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

View::View(int w, int h) {
}

View::~View() {
}

void View::reset() {
	directional_lights_.clear();
	point_lights_.clear();
	spot_lights_.clear();
}

VOLCANO_GRAPHICS_END
