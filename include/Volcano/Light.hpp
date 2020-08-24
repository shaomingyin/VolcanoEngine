//
//
#ifndef VOLCANO_LIGHT_HPP
#define VOLCANO_LIGHT_HPP

#include <QColor>

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>

VOLCANO_BEGIN

class VOLCANO_API Light: public Node
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Light(Node *parent = nullptr);
    ~Light(void) override;

public:
    const QColor &color(void) const;
    void setColor(const QColor &v);

signals:
    void colorChanged(void);

protected:

private:
    QColor m_color;
};

VOLCANO_INLINE const QColor &Light::color(void) const
{
    return m_color;
}

VOLCANO_INLINE void Light::setColor(const QColor &v)
{
    if (m_color != v)
    {
        m_color = v;
        colorChanged();
    }
}

VOLCANO_END

#endif // VOLCANO_LIGHT_HPP
