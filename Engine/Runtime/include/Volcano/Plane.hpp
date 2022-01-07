//
//
#ifndef VOLCANO_PLANE_HPP
#define VOLCANO_PLANE_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Shape.hpp>

VOLCANO_BEGIN

class Plane: public Shape {
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal z READ z WRITE setZ NOTIFY zChanged)
    Q_PROPERTY(qreal D READ D WRITE setD NOTIFY DChanged)

public:
    Plane(QObject *parent = nullptr);
    Plane(qreal x, qreal y, qreal z, qreal d, QObject *parent = nullptr);
    ~Plane(void) override;

public:
    qreal x(void) const;
    void setX(qreal v);
    qreal y(void) const;
    void setY(qreal v);
    qreal z(void) const;
    void setZ(qreal v);
    qreal d(void) const;
    void setD(qreal v);

signals:
    void xChanged(qreal v);
    void yChanged(qreal v);
    void zChanged(qreal v);
    void dChanged(qreal v);

private:
    qreal m_x;
    qreal m_y;
    qreal m_z;
    qreal m_d;
};

VOLCANO_END

#endif // VOLCANO_PLANE_HPP
