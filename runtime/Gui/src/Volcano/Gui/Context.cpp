//
//
#include <Volcano/Gui/Context.h>

VOLCANO_GUI_BEGIN

static sf::Vector2f VectorU2F(sf::Vector2u v) {
    return sf::Vector2f(float(v.x), float(v.y));
}

Context::Context(sf::RenderTarget& render_target)
    : render_target_(render_target)
    , background_(VectorU2F(render_target.getSize()))
    , current_object_(nullptr) {
    background_.setFillColor(sf::Color::Transparent);
    background_.setOutlineColor(sf::Color::Transparent);
}

void Context::makeCurrentObject(Object* p) {
    VOLCANO_ASSERT(p != nullptr);
    if (current_object_ != p) {
        VOLCANO_ASSERT(std::find(objects_.begin(), objects_.end(), p) != objects_.end());
        current_object_ = p;
    }
}

void Context::makeTopObject(Object* p) {
    VOLCANO_ASSERT(p != nullptr);
    if (p != objects_.back()) {
        objects_.remove(p);
        objects_.push_back(p);
    }
}

void Context::addObject(Object* p) {
    VOLCANO_ASSERT(p != nullptr);
    auto it = std::find(objects_.begin(), objects_.end(), p);
    if (it == objects_.end()) {
        objects_.push_back(p);
        current_object_ = p;
    }
}

void Context::removeObject(Object* p) {
    VOLCANO_ASSERT(p != nullptr);
    auto it = std::find(objects_.begin(), objects_.end(), p);
    if (it != objects_.end()) {
        objects_.erase(it);
        if (current_object_ == p) {
            if (objects_.empty()) {
                current_object_ = nullptr;
            } else {
                current_object_ = objects_.back();
            }
        }
    }
}

void Context::render() noexcept {
    render_target_.setActive();
    render_target_.draw(background_);
    for (Object* obj: objects_) {
        render_target_.draw(*obj);
    }
}

void Context::handleEvent(const sf::Event& event) noexcept {
    switch (event.type) {
    case sf::Event::Resized:
        onResized(event.size);
        break;
    default:
        break;
    }

    if (current_object_ != nullptr) {
        current_object_->handleEvent(event);
    }
}

void Context::onResized(const sf::Event::SizeEvent& event) noexcept {
    background_.setSize(VectorU2F(render_target_.getSize()));
}

VOLCANO_GUI_END
