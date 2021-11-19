//
//
#ifndef VOLCANO_GRAPHICS_LIGHT_HPP
#define VOLCANO_GRAPHICS_LIGHT_HPP

#include <QColor>

#include <Volcano/Game/Object.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Light: public Game::Object {
    Q_OBJECT
    Q_PROPERTY(float strength READ strength WRITE setStrength NOTIFY strengthChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Light(QObject *parent = nullptr);
    ~Light(void) override;

public:
    float strength(void) const;
    void setStrength(float v);
    const QColor &color(void) const;
    void setColor(const QColor &v);

signals:
    void strengthChanged(float v);
    void colorChanged(const QColor &v);

private:
    float m_strength;
    QColor m_color;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_LIGHT_HPP
