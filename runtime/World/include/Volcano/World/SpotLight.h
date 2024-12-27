//
//
#ifndef VOLCANO_WORLD_SPOTLIGHT_H
#define VOLCANO_WORLD_SPOTLIGHT_H

#include <Volcano/World/Common.h>
#include <Volcano/World/DirectionalLight.h>

VOLCANO_WORLD_BEGIN

class SpotLight: public DirectionalLight {
    Q_OBJECT

public:
    SpotLight(QObject* parent = nullptr);
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SPOTLIGHT_H
