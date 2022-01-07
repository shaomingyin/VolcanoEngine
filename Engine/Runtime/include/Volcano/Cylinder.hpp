//
//
#ifndef VOLCANO_CYLINDER_HPP
#define VOLCANO_CYLINDER_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Shape.hpp>

VOLCANO_BEGIN

class Cylinder: public Shape {
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)

public:
    Cylinder(QObject *parent = nullptr);
    Cylinder(qreal radius, qreal height, QObject *parent = nullptr);
    ~Cylinder(void) override;

public:
    qreal radius(void) const;
    void setRadius(qreal v);
    qreal height(void) const;
    void setHeight(qreal v);

signals:
    void radiusChanged(qreal v);
    void heightChanged(qreal v);

private:
    qreal m_radius;
    qreal m_height;
};

VOLCANO_END

#endif // VOLCANO_CYLINDER_HPP
