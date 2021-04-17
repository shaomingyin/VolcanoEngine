//
//
#include <Volcano/Input/Context.hpp>

VOLCANO_INPUT_BEGIN

bool Context::init(void)
{
    return true;
}

void Context::frame(float elapsed)
{
    VOLCANO_UNUSED(elapsed);

    Event event;

    while (pollEvent(event))
        handleEvent(event);
}

void Context::handleEvent(const Event &event)
{
    switch (event.type) {
    case Event::TypeKeyDown:
        break;
    case Event::TypeKeyUp:
        break;
    case Event::TypeMouseMove:
        break;
    case Event::TypeMouseButton:
        break;
    default:
        break;
    }
}

VOLCANO_INPUT_END
