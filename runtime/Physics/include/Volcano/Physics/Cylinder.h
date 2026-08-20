//
//
#ifndef VOLCANO_WORLD_CYLINDER_H
#define VOLCANO_WORLD_CYLINDER_H

#include <QVector3D>

#include <Volcano/World/Common.h>
#include <Volcano/World/Shape.h>

VOLCANO_WORLD_BEGIN

class Cylinder: public Shape {
    Q_OBJECT
    Q_PROPERTY(QVector3D size READ size WRITE resize NOTIFY sizeChanged)

public:
    Cylinder(QObject* parent = nullptr);

public:
    const QVector3D& size() const noexcept;
    void resize(const QVector3D& v) noexcept;

signals:
    void sizeChanged(const QVector3D& v);

private:
    QVector3D size_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CYLINDER_H
