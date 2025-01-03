//
//
#include <Volcano/Resource/Texture.h>

VOLCANO_RESOURCE_BEGIN

QDataStream &operator<<(QDataStream &s, const Texture& v) {
    s << v.pixmap();
    return s;
}

QDataStream &operator>>(QDataStream &s, Texture& v) {
    s >> v.pixmap();
    return s;
}

VOLCANO_RESOURCE_END
