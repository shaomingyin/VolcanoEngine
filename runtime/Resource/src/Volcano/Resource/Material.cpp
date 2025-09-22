//
//
#include <Volcano/Resource/Material.h>

VOLCANO_RESOURCE_BEGIN

QDataStream &operator<<(QDataStream &s, const Material& v) {
    return s;
}

QDataStream &operator>>(QDataStream &s, Material& v) {
    return s;
}

VOLCANO_RESOURCE_END
