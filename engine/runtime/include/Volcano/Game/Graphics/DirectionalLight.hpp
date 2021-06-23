//
//
#ifndef VOLCANO_GAME_GRAPHICS_DIRECTIONALLIGHT_HPP
#define VOLCANO_GAME_GRAPHICS_DIRECTIONALLIGHT_HPP

#include <QVector3D>

#include <Volcano/Game/Graphics/Common.hpp>
#include <Volcano/Game/Graphics/Light.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

class DirectionalLight: public Light {
    Q_OBJECT
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    DirectionalLight(QObject *parent = nullptr);
    ~DirectionalLight(void) override;

public:
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &v);

signals:
    void directionChanged(const QVector3D &v);

private:
    QVector3D m_direction;
};

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_DIRECTIONALLIGHT_HPP
