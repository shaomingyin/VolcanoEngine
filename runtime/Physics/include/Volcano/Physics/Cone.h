//
//
#ifndef VOLCANO_WORLD_CONE_H
#define VOLCANO_WORLD_CONE_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Shape.h>

VOLCANO_WORLD_BEGIN

class Cone: public Shape {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)

public:
    Cone(QObject* parent = nullptr);

public:
    float radius() const noexcept;
    void setRadius(float v) noexcept;
    float height() const noexcept;
    void setHeight(float v) noexcept;

signals:
    void radiusChanged(float v);
    void heightChanged(float v);

private:
    float radius_;
    float height_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CONE_H
