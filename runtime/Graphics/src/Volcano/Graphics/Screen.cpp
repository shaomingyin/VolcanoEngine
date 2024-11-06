//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/Screen.h>

VOLCANO_GRAPHICS_BEGIN

Screen::Screen(int w, int h)
	: imgui_(ImGui::CreateContext())
	, width_(w)
	, height_(h) {
	if (imgui_ == nullptr) {
		throw Error(Errc::InvalidContext);
	}
}

void Screen::resize(int w, int h) {
}

VOLCANO_GRAPHICS_END
