//
//
#include <Volcano/System/Client.hpp>

VOLCANO_SYSTEM_BEGIN

Client::Client(QObject *parent):
    Graphics::Viewable(parent)
{
}

Client::~Client(void)
{
}

void Client::buildVisibleSet(Graphics::VisibleSet &out, Graphics::Camera &cam)
{
}

VOLCANO_SYSTEM_END
