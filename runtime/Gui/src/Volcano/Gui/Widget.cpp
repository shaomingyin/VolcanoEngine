//
//
#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

Widget::Widget(Widget& parent)
    : Context::Object(parent.owner())
    , parent_(&parent) {
}

Widget::Widget(Context& owner)
    : Context::Object(owner)
    , parent_(nullptr) {
    owner.addObject(this);
}

Widget::~Widget() {
    try {
        if (parent_ == nullptr) {
            owner().removeObject(this);
        }
    } catch (...) {
    }
}

void Widget::setParent(Widget* p) {
    if (parent_ != nullptr) {
        parent_->children_.remove(this);
    } else {
        owner().removeObject(this);
    }

    parent_ = p;

    if (parent_ != nullptr) {
        if (&parent_->owner() != &owner()) {
            throw std::runtime_error("Invalid widget parent context.");
        }
        parent_->children_.push_back(this);
    } else {
        owner().addObject(this);
    }
}

void Widget::onPaint(sf::RenderTarget& target) noexcept {
}

void Widget::onResized(const sf::Event::SizeEvent& event) noexcept {
}

VOLCANO_GUI_END
