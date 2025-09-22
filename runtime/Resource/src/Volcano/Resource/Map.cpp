//
//
#include <Volcano/Resource/Map.h>

VOLCANO_RESOURCE_BEGIN

QDataStream &operator<<(QDataStream &s, const Map& v) {
    return s;
}

QDataStream &operator>>(QDataStream &s, Map& v) {
    return s;
}

VOLCANO_RESOURCE_END
