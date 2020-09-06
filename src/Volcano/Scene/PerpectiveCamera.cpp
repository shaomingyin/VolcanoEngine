//
//
#include <Volcano/Scene/PerpectiveCamera.hpp>

VOLCANO_SCENE_BEGIN

PerpectiveCamera::PerpectiveCamera(QObject *parent):
    Camera(parent),
    m_fov(90.0f)
{
}

PerpectiveCamera::~PerpectiveCamera(void)
{
}

VOLCANO_SCENE_END
