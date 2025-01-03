//
//
#ifndef VOLCANO_RESOURCE_MAP_H
#define VOLCANO_RESOURCE_MAP_H

#include <Volcano/Resource/Common.h>

VOLCANO_RESOURCE_BEGIN

class Map {
public:
};

QDataStream &operator<<(QDataStream &s, const Map& v);
QDataStream &operator>>(QDataStream &s, Map& v);

VOLCANO_RESOURCE_END

#endif // VOLCANO_RESOURCE_MAP_H
