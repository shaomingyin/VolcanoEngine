//
//
#include <Volcano/Gui/Object.hpp>

VOLCANO_GUI_BEGIN

Object::Object(Context& context, float width, float height, Object* parent)
    : Object(context, { width, height }, parent) {
}

Object::Object(Context& context, const sf::Vector2f& size, Object* parent)
    : context_(context)
    , flags_(FlagEnabled | FlagVisible)
    , parent_(nullptr)
    , size_(size) {
    setParent(parent);
}

Object::~Object() {
    try {
        setParent(nullptr);
    } catch (...) {
    }
}

void Object::setParent(Object* parent) {
    if (parent_ != nullptr) {
        parent_->onChildRemoved(this);
        parent_->children_.remove(this);
    }
    parent_ = parent;
    if (parent_ != nullptr) {
        parent_->children_.push_back(this);
        parent_->onChildAdded(this);
    }
}

sf::Vector2f Object::toLocal(float x, float y) const noexcept {
    return toLocal({ x, y });
}

sf::Vector2f Object::toLocal(const sf::Vector2f& v) const noexcept {
    auto ret = getInverseTransform().transformPoint(v);
    if (parent_ != nullptr) {
        return parent_->toLocal(ret);
    }
    return ret;
}

sf::FloatRect Object::toLocal(const sf::FloatRect& v) const noexcept {
    auto ret= getInverseTransform().transformRect(v);
    if (parent_ != nullptr) {
        return parent_->toLocal(ret);
    }
    return ret;
}

sf::FloatRect Object::toLocal(float left, float top, float width, float height) const noexcept {
    return toLocal({ left, top, width, height });
}

sf::Vector2f Object::toGlobal(float x, float y) const noexcept {
    return toGlobal({ x, y });
}

sf::Vector2f Object::toGlobal(const sf::Vector2f& v) const noexcept {
    auto ret = getTransform().transformPoint(v);
    if (parent_ != nullptr) {
        return parent_->toGlobal(ret);
    }
    return ret;
}

sf::FloatRect Object::toGlobal(const sf::FloatRect& v) const noexcept {
    auto ret = getTransform().transformRect(v);
    if (parent_ != nullptr) {
        return parent_->toGlobal(ret);
    }
    return ret;
}

sf::FloatRect Object::toGlobal(float left, float top, float width, float height) const noexcept {
    return toGlobal({ left, top, width, height });
}

void Object::toTop() {
    if (parent_ != nullptr) {
        parent_->children_.remove(this);
        parent_->children_.push_back(this);
    }
}

bool Object::event(const sf::Event& evt) {
    if (!(flags_ & FlagEnabled)) {
        return false;
    }

    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        if ((*it)->event(evt)) {
            return true;
        }
    }

    return onEvent(evt);
}

void Object::onChildAdded(Object* child) {

}

void Object::onChildRemoved(Object* child) {

}

bool Object::onEvent(const sf::Event& evt) {
    bool ret;
    switch (evt.type) {
    case sf::Event::MouseMoved:
        ret = handleMouseMoved(evt.mouseMove);
        break;
    case sf::Event::MouseButtonPressed:
        ret = handleMouseButtonPressed(evt.mouseButton);
        break;
    case sf::Event::MouseButtonReleased:
        ret = handleMouseButtonReleased(evt.mouseButton);
        break;
    default:
        ret = false;
        break;
    }
    return ret;
}

void Object::onMouseMoved(const sf::Event::MouseMoveEvent& evt) {
}

void Object::onMouseEntered() {
}

void Object::onMouseLeft() {
}

void Object::onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt) {
}

void Object::onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt) {
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!(flags_ & FlagVisible)) {
        return;
    }

    states.transform *= getTransform();
    onPaint(target, states);

    for (const auto& child : children_) {
        target.draw(*child, states);
    }
}

bool Object::handleMouseMoved(const sf::Event::MouseMoveEvent& evt) {
    auto& position = getPosition();
    sf::FloatRect rect(position.x, position.y, size_.x, size_.y);

    if (!rect.contains(float(evt.x), float(evt.y))) {
        if (flags_ & FlagMouseInside) {
            flags_ &= ~FlagMouseInside;
            onMouseLeft();
        }
        if (!(flags_ & FlagMouseGrabbed)) {
            return false;
        }
    }

    if (!(flags_ & FlagMouseInside)) {
        flags_ |= FlagMouseInside;
        onMouseEntered();
    }

    auto local_pos = toLocal(float(evt.x), float(evt.y));
    onMouseMoved({ int(local_pos.x), int(local_pos.y) });

    return true;
}

bool Object::handleMouseEntered() {
    return true;
}

bool Object::handleMouseLeft() {
    return true;
}

bool Object::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& evt) {
    auto& position = getPosition();

    if (toGlobal(0, 0, size_.x, size_.y).contains(float(evt.x), float(evt.y))) {
        auto local_pos = toLocal(float(evt.x), float(evt.y));
        onMouseButtonPressed({ evt.button, int(local_pos.x), int(local_pos.y) });
        return true;
    }

    return false;
}

bool Object::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& evt) {
    auto& position = getPosition();

    if (toGlobal(0, 0, size_.x, size_.y).contains(float(evt.x), float(evt.y))) {
        onMouseButtonReleased({ evt.button, evt.x - int(position.x), evt.y - int(position.y) });
        return true;
    }

    return false;
}

VOLCANO_GUI_END
