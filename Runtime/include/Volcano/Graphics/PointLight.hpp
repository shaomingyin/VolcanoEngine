//
//
#ifndef VOLCANO_GRAPHICS_POINTLIGHT_HPP
#define VOLCANO_GRAPHICS_POINTLIGHT_HPP

#include <QVector3D>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

class PointLight: public Light {
public:
    PointLight(void);
    ~PointLight(void) override;

public:
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    float radius(void) const;
    void setRadius(float v);

private:
    QVector3D m_position;
    float m_radius;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_POINTLIGHT_HPP
