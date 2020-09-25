//
//
#ifndef VOLCANO_GAME_LIGHT_HPP
#define VOLCANO_GAME_LIGHT_HPP

#include <QColor>
#include <QObject>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_API Light: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal strength READ strength WRITE setStrength NOTIFY strengthChanged)

public:
    Light(QObject *parent = nullptr);
    ~Light(void) override;

public:
    const QColor &color(void) const;
    void setColor(const QColor &r);
    qreal strength(void) const;
    void setStrength(qreal v);

signals:
    void colorChanged(const QColor &r);
    void strengthChanged(qreal v);

private:
    QColor m_color;
    qreal m_strength;
};

VOLCANO_INLINE const QColor &Light::color(void) const
{
    return m_color;
}

VOLCANO_INLINE void Light::setColor(const QColor &r)
{
    if (m_color != r)
    {
        m_color = r;
        colorChanged(r);
    }
}

VOLCANO_INLINE qreal Light::strength(void) const
{
    return m_strength;
}

VOLCANO_INLINE void Light::setStrength(qreal v)
{
    if (m_strength != v)
    {
        m_strength = v;
        strengthChanged(v);
    }
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_HPP
