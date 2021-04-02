//
//
#include <Volcano/Drivers/Input/Event.hpp>

VOLCANO_DRIVERS_INPUT_BEGIN

Event::Event(void)
{
}

Event::~Event(void)
{
}

std::string Event::name(void) const
{
    return "BaseInputEvent";
}

VOLCANO_DRIVERS_INPUT_END
