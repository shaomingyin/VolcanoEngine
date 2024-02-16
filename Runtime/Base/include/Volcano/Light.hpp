//
//
#ifndef VOLCANO_LIGHT_HPP
#define VOLCANO_LIGHT_HPP

#include <QColor>

#include <Volcano/Common.hpp>
#include <Volcano/Actor.hpp>

VOLCANO_BEGIN

class Light: public Actor {
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

VOLCANO_END

#endif // VOLCANO_LIGHT_HPP
