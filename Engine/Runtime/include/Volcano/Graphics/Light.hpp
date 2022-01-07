//
//
#ifndef VOLCANO_GRAPHICS_LIGHT_HPP
#define VOLCANO_GRAPHICS_LIGHT_HPP

#include <QColor>

#include <Volcano/Game/Actor.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Light: public Game::Actor {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Light(QObject *parent = nullptr);
    ~Light(void) override;

public:
    const QColor &color(void) const;
    void setColor(const QColor &v);

signals:
    void colorChanged(const QColor &v);

private:
    QColor m_color;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_LIGHT_HPP
