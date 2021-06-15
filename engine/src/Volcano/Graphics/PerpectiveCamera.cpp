//
//
#include <Volcano/Graphics/PerpectiveCamera.hpp>

VOLCANO_GRAPHICS_BEGIN

PerpectiveCamera::PerpectiveCamera(QObject *parent):
    Camera(parent)
{
}

PerpectiveCamera::~PerpectiveCamera(void)
{
}

float PerpectiveCamera::fov(void) const
{
    return m_fov;
}

void PerpectiveCamera::setFov(float v)
{
    if (!qFuzzyCompare(m_fov, v)) {
        m_fov = qBound(1.0f, v, 179.0f);
        emit fovChanged(m_fov);
    }
}

void PerpectiveCamera::calcProjectMatrix(QMatrix4x4 &out)
{
    const auto &r = rect();
    out.perspective(m_fov, r.width() / r.height(), nearPlane(), farPlane());
}

VOLCANO_GRAPHICS_END
