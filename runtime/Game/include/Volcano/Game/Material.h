//
//
#ifndef VOLCANO_GAME_MATERIAL_H
#define VOLCANO_GAME_MATERIAL_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Component.h>

VOLCANO_GAME_BEGIN

class Material: public Component {
public:
    Material(QObject* parent = nullptr);

public:
    friend QDataStream& operator<<(QDataStream& s, const Material& v);
    friend QDataStream& operator>>(QDataStream& s, Material& v);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MATERIAL_H
