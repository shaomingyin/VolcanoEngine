//
//
#include <Volcano/Resource/Mesh.h>

VOLCANO_RESOURCE_BEGIN

size_t Mesh::vertexSize(quint32 attributes) {
    quint32 size = 0;
    if (attributes & AttributePosition) {
        size += sizeof(float) * 3;
    }
    if (attributes & AttributeColor) {
        size += sizeof(float) * 3;
    }
    if (attributes & AttributeNormal) {
        size += sizeof(float) * 3;
    }
    if (attributes & AttributeTexCoord) {
        size += sizeof(float) * 2;
    }
    return size;
}

QDataStream &operator<<(QDataStream &s, const Mesh& v) {
    s << v.attributes;
    s << v.vertices;
    s << v.indices;
    return s;
}

QDataStream &operator>>(QDataStream &s, Mesh& v) {
    s >> v.attributes;
    s >> v.vertices;
    s >> v.indices;
    return s;
}

VOLCANO_RESOURCE_END
