//
//
#include <Volcano/Gui/MenuBar.h>

VOLCANO_GUI_BEGIN

MenuBar::MenuBar(Object* parent)
	: MenuBar({}, parent) {
}

MenuBar::MenuBar(std::vector<Menu>&& menus, Object* parent)
	: Object(parent)
	, menus_(std::move(menus)) {
}

void MenuBar::paint() {
	if (menus_.empty()) {
		return;
	}
	bool begin;
	if (parent() != nullptr) {
		begin = ImGui::BeginMenuBar();
	} else {
		begin = ImGui::BeginMainMenuBar();
	}
	if (begin) {
		for (auto& menu : menus_) {
			menu.update();
		}
		if (parent() != nullptr) {
			ImGui::EndMenuBar();
		} else {
			ImGui::EndMainMenuBar();
		}
	}
}

VOLCANO_GUI_END
