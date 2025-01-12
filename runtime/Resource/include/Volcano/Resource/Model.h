//
//
#ifndef VOLCANO_RESOURCE_MODEL_H
#define VOLCANO_RESOURCE_MODEL_H

#include <QMap>
#include <QList>

#include <Volcano/Resource/Common.h>
#include <Volcano/Resource/Mesh.h>
#include <Volcano/Resource/Material.h>
#include <Volcano/Resource/Texture.h>

VOLCANO_RESOURCE_BEGIN

struct ModelData {
    QList<Mesh> mesh_list;
    QList<Texture> texture_list;
    QList<Material> material_list;
};

using Model = QMap<DetailLevel, ModelData>;

QDataStream &operator<<(QDataStream &s, const Model& v);
QDataStream &operator>>(QDataStream &s, Model& v);

VOLCANO_RESOURCE_END

#endif // VOLCANO_RESOURCE_MODEL_H
