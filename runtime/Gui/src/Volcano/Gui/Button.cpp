//
//
#include <Volcano/Gui/Button.h>

VOLCANO_GUI_BEGIN

Button::Button(std::string label, Object* parent)
	: Object(parent)
	, label_(std::move(label)) {
}

void Button::paint() {
	ImGui::Button(label_.c_str(), { 100, 30 });
}

VOLCANO_GUI_END
