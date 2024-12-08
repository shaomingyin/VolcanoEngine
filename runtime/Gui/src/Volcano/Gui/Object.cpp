//
//
#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

Object::Object(Object* parent)
	: parent_(nullptr) {
	setParent(parent);
}

Object::~Object() {
	setParent(nullptr);
}

void Object::setParent(Object* p) {
	if (parent_ != nullptr) {
		parent_->removeChild(this);
	}
	parent_ = p;
	if (parent_ != nullptr) {
		parent_->addChild(this);
	}
}

void Object::update() {
	if (enabled_ && visible_) {
		paint();
	}
}

VOLCANO_GUI_END
