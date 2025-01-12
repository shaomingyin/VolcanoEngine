//
//
#ifndef VOLCANO_RESOURCE_MESH_H
#define VOLCANO_RESOURCE_MESH_H

#include <QColor>
#include <QVector>

#include <Volcano/Resource/Common.h>

VOLCANO_RESOURCE_BEGIN

struct Mesh {
    enum {
        VertexAttributePosition = 0x1,
        VertexAttributeColor = 0x2,
        VertexAttributeNormal = 0x4,
        VertexAttributeTexCoord = 0x8
    };

    size_t vertexSize() const {
        return vertexSize(attributes);
    }

    static size_t vertexSize(quint32 attributes);

    quint32 attributes;
    QVector<float> vertices;
    QVector<quint32> indices;
};

QDataStream &operator<<(QDataStream &s, const Mesh& v);
QDataStream &operator>>(QDataStream &s, Mesh& v);

VOLCANO_RESOURCE_END

#endif // VOLCANO_RESOURCE_MESH_H
