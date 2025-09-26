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

public:
    friend QDataStream& operator<<(QDataStream& s, const Material& v);
    friend QDataStream& operator>>(QDataStream& s, Material& v);
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MATERIAL_H
