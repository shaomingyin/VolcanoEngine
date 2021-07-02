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

void Component::update(float elapsed)
{
}

VOLCANO_GAME_END
