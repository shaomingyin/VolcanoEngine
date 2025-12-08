//
//
#ifndef VOLCANO_GAME_POINTLIGHT_H
#define VOLCANO_GAME_POINTLIGHT_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Light.h>

VOLCANO_GAME_BEGIN

class PointLight: public Light {
    Q_OBJECT

public:
    PointLight(QObject* parent = nullptr);

public:
    friend QDataStream& operator<<(QDataStream& s, const PointLight& v);
    friend QDataStream& operator>>(QDataStream& s, PointLight& v);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_POINTLIGHT_H
