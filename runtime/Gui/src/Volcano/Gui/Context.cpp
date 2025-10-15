//
//
#include <Volcano/Gui/Context.h>

VOLCANO_GUI_BEGIN

// Context::Object

Context::Object::Object(Context& owner)
    : owner_(owner)
    , flags_(FlagEnabled | FlagVisible) {
}

void Context::Object::paint(sf::RenderTarget& target) noexcept {
    if (isVisible()) {
        onPaint(target);
    }
}

void Context::Object::event(const sf::Event& event) noexcept {
    if (!isEnabled()) {
        return;
    }

    switch (event.type) {
    case sf::Event::Resized:
        onResized(event.size);
        break;
    default:
        break;
    }
}

// Context

static sf::Vector2f VectorU2F(sf::Vector2u v) {
    return sf::Vector2f(float(v.x), float(v.y));
}

Context::Context(sf::RenderTarget& render_target)
    : render_target_(render_target)
    , background_(VectorU2F(render_target.getSize())) {
    background_.setFillColor(sf::Color::Transparent);
    background_.setOutlineColor(sf::Color::Transparent);
}

void Context::addObject(Object* p) {
    VOLCANO_ASSERT(p != nullptr);
    auto it = std::find(objects_.begin(), objects_.end(), p);
    if (it == objects_.end()) {
        objects_.push_back(p);
    }
}

void Context::removeObject(Object* p) {
    VOLCANO_ASSERT(p != nullptr);
    auto it = std::find(objects_.begin(), objects_.end(), p);
    if (it != objects_.end()) {
        objects_.erase(it);
    }
}

void Context::moveToTop(Object* p) {
    VOLCANO_ASSERT(p != nullptr);
    auto it = std::find(objects_.begin(), objects_.end(), p);
    if (it != objects_.end()) {
        objects_.erase(it);
        objects_.push_back(p);
    }
}

void Context::paint() noexcept {
    render_target_.setActive();
    render_target_.draw(background_);
    for (Object* obj: objects()) {
        obj->paint(render_target_);
    }
}

void Context::event(const sf::Event& event) noexcept {
    for (Object* obj: objects()) {
        obj->event(event);
    }
}

VOLCANO_GUI_END
