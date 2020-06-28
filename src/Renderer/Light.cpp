//
//
#include <Volcano/Renderer/Light.h>

VOLCANO_RENDERER_BEGIN

Light::Light(QObject *parent):
    Node(parent)
{
    setProgress(1.0);
    setState(StateReady);
}

Light::~Light(void)
{
}

VOLCANO_RENDERER_END
