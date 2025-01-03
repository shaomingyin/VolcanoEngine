//
//
#ifndef VOLCANO_RESOURCE_MODEL_H
#define VOLCANO_RESOURCE_MODEL_H

#include <QList>
#include <QPixmap>

#include <Volcano/Resource/Common.h>
#include <Volcano/Resource/Mesh.h>
#include <Volcano/Resource/Material.h>

VOLCANO_RESOURCE_BEGIN

class Model {
public:
    Model() = default;

public:
    QList<Mesh>& meshList() {
        return mesh_list_;
    }

    const QList<Mesh>& meshList() const {
        return mesh_list_;
    }

    QList<QPixmap>& textureList() {
        return texture_list_;
    }

    const QList<QPixmap> textureList() const {
        return texture_list_;
    }

    QList<Material>& materialList() {
        return material_list_;
    }

    const QList<Material> materialList() const {
        return material_list_;
    }

private:
    QList<Mesh> mesh_list_;
    QList<QPixmap> texture_list_;
    QList<Material> material_list_;
};

QDataStream &operator<<(QDataStream &s, const Model& v);
QDataStream &operator>>(QDataStream &s, Model& v);

VOLCANO_RESOURCE_END

#endif // VOLCANO_RESOURCE_MODEL_H
