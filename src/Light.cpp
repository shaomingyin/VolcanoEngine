//
//
#include <Volcano/Light.h>

VOLCANO_BEGIN

Light::Light(QObject *parent):
    Node(parent)
{
    setProgress(1.0);
    setState(StateReady);
}

Light::~Light(void)
{
}

VOLCANO_END
