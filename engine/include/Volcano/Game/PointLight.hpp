//
//
#ifndef VOLCANO_GAME_POINTLIGHT_HPP
#define VOLCANO_GAME_POINTLIGHT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

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

VOLCANO_GAME_END

#endif // VOLCANO_GAME_POINTLIGHT_HPP
