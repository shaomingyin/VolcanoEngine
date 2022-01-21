//
//
#ifndef VOLCANO_GRAPHICS_DIRECTIONALLIGHT_HPP
#define VOLCANO_GRAPHICS_DIRECTIONALLIGHT_HPP

#include <QVector3D>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Light.hpp>

#ifdef VOLCANO_GRAPHICS

VOLCANO_GRAPHICS_BEGIN

class DirectionalLight: public Light {
public:
    DirectionalLight(void);
    virtual ~DirectionalLight(void);

public:
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &v);

private:
    QVector3D m_direction;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS

#endif // VOLCANO_GRAPHICS_DIRECTIONALLIGHT_HPP
