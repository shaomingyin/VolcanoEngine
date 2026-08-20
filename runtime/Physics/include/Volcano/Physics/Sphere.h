//
//
#ifndef VOLCANO_WORLD_SPHERE_H
#define VOLCANO_WORLD_SPHERE_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Shape.h>

VOLCANO_WORLD_BEGIN

class Sphere: public Shape {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    Sphere(QObject* parent = nullptr);

public:
    float radius() const noexcept;
    void setRadius(float v) noexcept;

signals:
    void radiusChanged(float v);

private:
    float radius_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SPHERE_H
