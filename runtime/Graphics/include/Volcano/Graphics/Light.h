//
//
#ifndef VOLCANO_WORLD_LIGHT_H
#define VOLCANO_WORLD_LIGHT_H

#include <QColor>
#include <QObject>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

class Light: public Transformable {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float strength READ strength WRITE setStrength NOTIFY strengthChanged)

public:
    Light(QObject* parent = nullptr);

public:
    const QColor& color() const noexcept;
    void setColor(const QColor& v) noexcept;
    float strength() const noexcept;
    void setStrength(float v) noexcept;

signals:
    void colorChanged(const QColor& v);
    void strengthChanged(float v);

private:
    QColor color_;
	float strength_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_LIGHT_H
