//
//
#ifndef VOLCANO_RESOURCE_MATERIAL_H
#define VOLCANO_RESOURCE_MATERIAL_H

#include <Volcano/Resource/Common.h>

VOLCANO_RESOURCE_BEGIN

struct Material {
};

QDataStream &operator<<(QDataStream &s, const Material& v);
QDataStream &operator>>(QDataStream &s, Material& v);

VOLCANO_RESOURCE_END

#endif // VOLCANO_RESOURCE_MATERIAL_H
