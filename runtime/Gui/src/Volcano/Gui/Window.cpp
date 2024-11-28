//
//
#include <Volcano/Gui/Window.h>

VOLCANO_GUI_BEGIN

Window::Window(const std::string& title, Rect rect, int flags)
	: title_(title)
	, flags_(flags)
	, rect_(rect) {
}

void Window::update() {
}

VOLCANO_GUI_END
