//
//
#ifndef VOLCANO_GAME_GRAPHICS_POINTLIGHT_HPP
#define VOLCANO_GAME_GRAPHICS_POINTLIGHT_HPP

#include <QVector3D>

#include <Volcano/Game/Graphics/Common.hpp>
#include <Volcano/Game/Graphics/Light.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

class PointLight: public Light {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    PointLight(QObject *parent = nullptr);
    ~PointLight(void) override;

public:
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    float radius(void) const;
    void setRadius(float v);

signals:
    void positionChanged(const QVector3D &v);
    void radiusChanged(float v);

private:
    QVector3D m_position;
    float m_radius;
};

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_GRAPHICS_POINTLIGHT_HPP
