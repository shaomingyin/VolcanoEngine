//
//
#ifndef VOLCANO_GAME_LIGHT_HPP
#define VOLCANO_GAME_LIGHT_HPP

#include <QVector3D>
#include <QObject>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API Light: public QObject {
    Q_OBJECT

public:
    Light(QObject *parent = nullptr);
    ~Light(void) override;

public:
    float strength(void) const;
    void setStrength(float v);
    const QVector3D &color(void) const;
    void setColor(const QVector3D &r);

private:
    float m_strength;
    QVector3D m_color;
};

VOLCANO_INLINE float Light::strength(void) const
{
    return m_strength;
}

VOLCANO_INLINE void Light::setStrength(float v)
{
    m_strength = qBound(0.0f, v, 1.0f);
}

VOLCANO_INLINE const QVector3D &Light::color(void) const
{
    return m_color;
}

VOLCANO_INLINE void Light::setColor(const QVector3D &r)
{
    m_color = r;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_HPP
