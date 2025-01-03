//
//
#include <Volcano/Resource/Mesh.h>

VOLCANO_RESOURCE_BEGIN

QDataStream &operator<<(QDataStream &s, const Mesh& v) {
    s << v.vertexData();
    s << v.indexData();
    s << v.colorData();
    s << v.texCoordData();
    return s;
}

QDataStream &operator>>(QDataStream &s, Mesh& v) {
    s >> v.vertexData();
    s >> v.indexData();
    s >> v.colorData();
    s >> v.texCoordData();
    return s;
}

VOLCANO_RESOURCE_END
