//
//
#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

Widget::Widget(float width, float height, Widget* parent)
    : Object(width, height)
    , parent_(parent) {
}

Widget::~Widget() {
    try {
        setParent(nullptr);
    } catch (...) {
    }
}

void Widget::setParent(Widget* p) {
    if (parent_ != nullptr) {
        parent_->children_.remove(this);
    }
    parent_ = p;
    if (parent_ != nullptr) {
        parent_->children_.push_back(this);
    }
}

bool Widget::handleEvent(const sf::Event& event) noexcept {
    for (auto child: children_) {
        if (child->handleEvent(event)) {
            return true;
        }
    }
    return Object::handleEvent(event);
}

void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Object::draw(target, states);
    for (auto child: children_) {
        child->draw(target, states);
    }
}

VOLCANO_GUI_END
