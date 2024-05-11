//
//
#ifndef VOLCANO_GAME_LIGHT_HPP
#define VOLCANO_GAME_LIGHT_HPP

#include <QColor>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API Light: public Actor {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Light(QObject* parent = nullptr);
    Light(Context& context, QObject* parent = nullptr);

public:
    const QColor& color(void) const;
    void setColor(const QColor& v);

signals:
    void colorChanged(const QColor& v);

private:
    QColor color_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_HPP
