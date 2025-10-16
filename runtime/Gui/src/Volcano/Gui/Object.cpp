//
//
#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

Object::Object(float width, float height)
    : flags_(FlagEnabled | FlagVisible)
    , background_({ width, height }) {
    background_.setFillColor(sf::Color::Transparent);
    background_.setOutlineColor(sf::Color::Transparent);
}

void Object::resize(const sf::Vector2f& v) noexcept {
    background_.setSize(v);
    sf::Event::SizeEvent event;
    event.width = v.x;
    event.height = v.y;
    onResized(event);
}

bool Object::handleEvent(const sf::Event& event) noexcept {
    if (!isEnabled()) {
        return false;
    }

    bool ret;

    switch (event.type) {
    case sf::Event::Resized:
        ret = onResized(event.size);
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (isVisible()) {
        states.transform.combine(transform_);
        onPaint(target, states);
    }
}

void Object::onPaint(sf::RenderTarget& target, sf::RenderStates states) const noexcept {
    target.draw(background_, states);
}

bool Object::onResized(const sf::Event::SizeEvent& event) noexcept {
    return false;
}

VOLCANO_GUI_END
