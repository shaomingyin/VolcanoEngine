//
//
#ifndef VOLCANO_GAME_DIRECTIONALLIGHT_HPP
#define VOLCANO_GAME_DIRECTIONALLIGHT_HPP

#include <QVector3D>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API DirectionalLight: public Light {
    Q_OBJECT

public:
    DirectionalLight(QObject *parent = nullptr);
    ~DirectionalLight(void) override;

public:
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &r);

private:
    QVector3D m_direction;
};

VOLCANO_INLINE const QVector3D &DirectionalLight::direction(void) const
{
    return m_direction;
}

VOLCANO_INLINE void DirectionalLight::setDirection(const QVector3D &r)
{
    m_direction = r;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DIRECTIONALLIGHT_HPP
