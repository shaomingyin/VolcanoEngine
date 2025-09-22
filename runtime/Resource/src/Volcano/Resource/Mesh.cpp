//
//
#include <Volcano/Resource/Mesh.h>

VOLCANO_RESOURCE_BEGIN

size_t Mesh::vertexSize(quint32 attributes) {
    quint32 size = 0;
    if (attributes & VertexAttributePosition) {
        size += sizeof(float) * 3;
    }
    if (attributes & VertexAttributeColor) {
        size += sizeof(float) * 3;
    }
    if (attributes & VertexAttributeNormal) {
        size += sizeof(float) * 3;
    }
    if (attributes & VertexAttributeTexCoord) {
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
