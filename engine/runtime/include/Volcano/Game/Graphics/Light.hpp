//
//
#ifndef VOLCANO_GAME_GRAPHICS_LIGHT_HPP
#define VOLCANO_GAME_GRAPHICS_LIGHT_HPP

#include <QColor>

#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Graphics/Common.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

class Light: public Object {
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

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_GRAPHICS_LIGHT_HPP
