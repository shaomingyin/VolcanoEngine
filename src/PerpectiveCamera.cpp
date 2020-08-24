//
//
#include <Volcano/PerpectiveCamera.hpp>

VOLCANO_BEGIN

PerpectiveCamera::PerpectiveCamera(Node *parent):
    Camera(parent),
    m_fov(90.0f)
{
}

PerpectiveCamera::~PerpectiveCamera(void)
{
}

VOLCANO_END
