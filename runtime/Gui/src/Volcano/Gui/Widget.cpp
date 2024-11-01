//
//
#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

Widget::Widget(Widget& parent)
	: context_(parent.context())
	, parent_(&parent) {
}

Widget::Widget(Context& context)
	: context_(context)
	, parent_(nullptr) {
	context_.addWidget(this);
}

Widget::~Widget() {
	if (parent_ == nullptr) {
		context_.removeWidget(this);
	}
}

void Widget::handleEvent(const SDL_Event& evt) {
}

VOLCANO_GUI_END
