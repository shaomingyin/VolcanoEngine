//
//
#ifndef VOLCANO_WORLDAMBIENT_HPP
#define VOLCANO_WORLDAMBIENT_HPP

#include <QColor>
#include <QObject>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API WorldAmbient: public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal strength READ strength WRITE setStrength NOTIFY strengthChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    WorldAmbient(QObject *parent = nullptr);
    ~WorldAmbient(void) override;

public:
    qreal strength(void) const;
    void setStrength(qreal v);
    const QColor &color(void) const;
    void setColor(const QColor &v);

signals:
    void strengthChanged(qreal v);
    void colorChanged(const QColor &v);

private:
    qreal m_strength;
    QColor m_color;
};

VOLCANO_INLINE qreal WorldAmbient::strength(void) const
{
    return m_strength;
}

VOLCANO_INLINE void WorldAmbient::setStrength(qreal v)
{
    if (m_strength != v)
    {
        m_strength = v;
        strengthChanged(v);
    }
}

VOLCANO_INLINE const QColor &WorldAmbient::color(void) const
{
    return m_color;
}

VOLCANO_INLINE void WorldAmbient::setColor(const QColor &v)
{
    if (m_color != v)
    {
        m_color = v;
        colorChanged(v);
    }
}

VOLCANO_END

#endif // VOLCANO_WORLDAMBIENT_HPP
