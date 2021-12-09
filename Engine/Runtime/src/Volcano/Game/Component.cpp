//
//
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

Component::Component(QObject *parent):
    QObject(parent)
{
}

Component::~Component(void)
{
}

void Component::tick(Duration elapsed)
{
}

VOLCANO_GAME_END
