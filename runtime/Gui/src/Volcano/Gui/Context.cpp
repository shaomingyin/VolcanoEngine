//
//
#include <Volcano/Gui/Context.h>

VOLCANO_GUI_BEGIN

void Context::addWidget(WidgetBase* p) {
	widgets_.push_back(p);
}

void Context::removeWidget(WidgetBase* p) {
	for (auto it = widgets_.begin(); it != widgets_.end(); ++it) {
		if (p == (*it)) {
			widgets_.erase(it);
			return;
		}
	}
}

VOLCANO_GUI_END
