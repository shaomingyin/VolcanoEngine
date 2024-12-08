//
//
#include <Volcano/Gui/MenuItem.h>

VOLCANO_GUI_BEGIN

MenuItem::MenuItem(std::string&& label, bool* selected, Object* parent)
	: Object(parent)
	, label_(std::move(label))
	, selected_(selected) {
}

void MenuItem::paint() {
	ImGui::MenuItem(label_.c_str(), nullptr, selected_, isEnabled());
}

VOLCANO_GUI_END
