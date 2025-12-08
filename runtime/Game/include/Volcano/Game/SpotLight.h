//
//
#ifndef VOLCANO_GAME_SPOTLIGHT_H
#define VOLCANO_GAME_SPOTLIGHT_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/DirectionalLight.h>

VOLCANO_GAME_BEGIN

class SpotLight: public DirectionalLight {
    Q_OBJECT

public:
    SpotLight(QObject* parent = nullptr);

public:
    friend QDataStream& operator<<(QDataStream& s, const SpotLight& v);
    friend QDataStream& operator>>(QDataStream& s, SpotLight& v);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SPOTLIGHT_H
