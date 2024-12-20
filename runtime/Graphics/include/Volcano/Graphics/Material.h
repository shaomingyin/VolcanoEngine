//
//
#ifndef VOLCANO_GRAPHICS_MATERIAL_H
#define VOLCANO_GRAPHICS_MATERIAL_H

#include <Volcano/World/Component.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Material: public World::Component {
    Q_OBJECT

public:
    Material(QObject* parent = nullptr);
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MATERIAL_H
