//
//
#ifndef VOLCANO_POINTLIGHT_HPP
#define VOLCANO_POINTLIGHT_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

class PointLight: public Light {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    PointLight(QObject *parent = nullptr);
    ~PointLight(void) override;

public:
    float radius(void) const;
    void setRadius(float v);

signals:
    void radiusChanged(float v);

private:
    float m_radius;
};

VOLCANO_END

#endif // VOLCANO_POINTLIGHT_HPP
