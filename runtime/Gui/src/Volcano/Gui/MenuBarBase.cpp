//
//
#include <Volcano/Gui/MenuBar.h>

VOLCANO_GUI_BEGIN

MenuBar::MenuBar(Context& context, std::vector<Menu>&& menus, Object* parent)
	: Object(context, parent)
	, menus_(std::move(menus)) {
}

void MenuBar::paint() {
	if (!menus_.empty() && ImGui::BeginMenuBar()) {
		for (auto& menu : menus_) {
			menu.update();
		}
		ImGui::EndMenuBar();
	}
}

VOLCANO_GUI_END
