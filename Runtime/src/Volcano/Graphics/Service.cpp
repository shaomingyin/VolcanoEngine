//
//
#include <Volcano/Graphics/Service.hpp>

#ifdef VOLCANO_GRAPHICS

VOLCANO_GRAPHICS_BEGIN

Service::Service(QObject *parent):
    Volcano::Service(parent)
{
}

Service::~Service(void)
{
}

void Service::enableClear(void)
{
    m_isClearEnabled = true;
}

void Service::disableClear(void)
{
    m_isClearEnabled = false;
}

void Service::setClearColor(const QColor &v)
{
    m_clearColor = v;
}

void Service::lookAt(const QVector3D &position, const QVector3D &direction, const QVector3D &up)
{
    m_position = position;
    m_direction = direction;
    m_up = up;
}

void Service::perspective(float fov, float ratio, float nearPlane, float farPlane)
{
    Q_ASSERT(nearPlane < 0.0f);
    Q_ASSERT(farPlane < 0.0f);
    Q_ASSERT(farPlane < nearPlane);

    m_fov = fov;
    m_ratio = ratio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void Service::setAmbientLight(const QColor &v)
{
    m_ambientLight = v;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS
