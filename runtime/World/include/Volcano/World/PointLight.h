//
//
#ifndef VOLCANO_WORLD_POINTLIGHT_H
#define VOLCANO_WORLD_POINTLIGHT_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class PointLight: public Light {
    Q_OBJECT

public:
    PointLight(QObject* parent = nullptr);

public:
    friend QDataStream& operator<<(QDataStream& s, const PointLight& v);
    friend QDataStream& operator>>(QDataStream& s, PointLight& v);
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_POINTLIGHT_H
