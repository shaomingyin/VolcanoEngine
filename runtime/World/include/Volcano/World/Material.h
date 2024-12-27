//
//
#ifndef VOLCANO_WORLD_MATERIAL_H
#define VOLCANO_WORLD_MATERIAL_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Component.h>

VOLCANO_WORLD_BEGIN

class Material: public Component {
public:
    Material(QObject* parent = nullptr);
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MATERIAL_H
