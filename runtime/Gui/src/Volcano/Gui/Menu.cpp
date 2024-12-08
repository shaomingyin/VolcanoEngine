//
//
#include <Volcano/Gui/Menu.h>

VOLCANO_GUI_BEGIN

Menu::Menu(std::string&& label, std::vector<MenuItem>&& items, Object* parent)
	: Object(parent)
	, label_(std::move(label))
	, items_(std::move(items)) {
}

void Menu::paint() {
	if (ImGui::BeginMenu(label_.c_str(), isEnabled())) {
		for (auto& item : items_) {
			item.update();
		}
		ImGui::EndMenu();
	}
}

VOLCANO_GUI_END
