//
//
#include <Volcano/System/Console.h>

VOLCANO_SYSTEM_BEGIN

Console::Console(Gui::Rect rect)
	: Gui::Window("Console", rect, FlagFrameLess) {
}

void Console::update() {
	auto curr_pos = pos();
	auto curr_size = size();

	if (toggle_) {
		if (curr_pos.y() < (curr_size.height() / 2)) {
			curr_pos.setY(curr_pos.y() + 1.0f);
		}
	} else {
		if (curr_pos.y() > -(curr_size.height() / 2)) {
			curr_pos.setY(curr_pos.y() - 1.0f);
		}
	}

	Gui::Window::update();
}

VOLCANO_SYSTEM_END
