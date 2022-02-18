//
//
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

Component::Component(QObject *parent):
    Object(parent)
{
}

Component::Component(Context *context, QObject *parent):
    Object(context, parent)
{
}

Component::~Component(void)
{
}

VOLCANO_GAME_END
