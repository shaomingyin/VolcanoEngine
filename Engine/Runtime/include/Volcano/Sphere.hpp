//
//
#ifndef VOLCANO_SPHERE_HPP
#define VOLCANO_SPHERE_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Shape.hpp>

VOLCANO_BEGIN

class Sphere: public Shape {
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    Sphere(QObject *parent = nullptr);
    Sphere(qreal radius, QObject *parent = nullptr);
    ~Sphere(void) override;

public:
    qreal radius(void) const;
    void setRadius(qreal v);

signals:
    void radiusChanged(qreal v);

private:
    qreal m_radius;
};

VOLCANO_END

#endif // VOLCANO_SPHERE_HPP
