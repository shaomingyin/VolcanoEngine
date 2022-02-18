//
//
#ifndef VOLCANO_GAME_LIGHT_HPP
#define VOLCANO_GAME_LIGHT_HPP

#include <QColor>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

class Light: public Actor {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Light(QObject *parent = nullptr);
    Light(Context *context, QObject *parent = nullptr);
    ~Light(void) override;

public:
    const QColor &color(void) const;
    void setColor(const QColor &v);

signals:
    void colorChanged(const QColor &v);

private:
    QColor m_color;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_HPP
