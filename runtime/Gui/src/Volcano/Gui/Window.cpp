//
//
#include <Volcano/Error.h>
#include <Volcano/Gui/Window.h>

VOLCANO_GUI_BEGIN

Window::Window(NVGcontext* nvg)
	: nvg_(nvg) {
	if (nvg_ == nullptr) {
		throw Error(Errc::OutOfResource);
	}
}

void Window::render() {
}

NVGcontext* Window::nvg() {
	return nvg_;
}

void Window::handleEvent(const SDL_Event& evt) {
	switch (evt.type) {
	}

	auto& ws = widgets();
	for (auto& w: ws) {
	}
}

VOLCANO_GUI_END
