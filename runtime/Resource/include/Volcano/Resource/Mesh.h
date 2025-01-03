//
//
#ifndef VOLCANO_RESOURCE_MESH_H
#define VOLCANO_RESOURCE_MESH_H

#include <QColor>
#include <QVector>

#include <Volcano/Resource/Common.h>

VOLCANO_RESOURCE_BEGIN

class Mesh {
public:
    Mesh() = default;

public:
    QVector<Vector3f>& vertexData() {
        return vertex_data_;
    }

    const QVector<Vector3f>& vertexData() const {
        return vertex_data_;
    }

    QVector<quint32>& indexData() {
        return index_data_;
    }

    const QVector<quint32>& indexData() const {
        return index_data_;
    }

    QVector<QColor>& colorData() {
        return color_data_;
    }

    const QVector<QColor>& colorData() const {
        return color_data_;
    }

    QVector<Vector2f>& texCoordData() {
        return tex_coord_data_;
    }

    const QVector<Vector2f>& texCoordData() const {
        return tex_coord_data_;
    }

private:
    QVector<Vector3f> vertex_data_;
    QVector<quint32> index_data_;
    QVector<QColor> color_data_;
    QVector<Vector2f> tex_coord_data_;
};

QDataStream &operator<<(QDataStream &s, const Mesh& v);
QDataStream &operator>>(QDataStream &s, Mesh& v);

VOLCANO_RESOURCE_END

#endif // VOLCANO_RESOURCE_MESH_H
