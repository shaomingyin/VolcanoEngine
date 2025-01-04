//
//
#ifndef VOLCANO_RESOURCE_MODEL_H
#define VOLCANO_RESOURCE_MODEL_H

#include <memory>

#include <QList>

#include <Volcano/Resource/Common.h>
#include <Volcano/Resource/Mesh.h>
#include <Volcano/Resource/Material.h>
#include <Volcano/Resource/Texture.h>

VOLCANO_RESOURCE_BEGIN

struct Model {
    QList<Mesh> mesh_list;
    QList<Texture> texture_list;
    QList<Material> material_list;
};

QDataStream &operator<<(QDataStream &s, const Model& v);
QDataStream &operator>>(QDataStream &s, Model& v);

VOLCANO_RESOURCE_END

#endif // VOLCANO_RESOURCE_MODEL_H
