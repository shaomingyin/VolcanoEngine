//
//
#include <Volcano/Resource/Model.h>

VOLCANO_RESOURCE_BEGIN

QDataStream &operator<<(QDataStream &s, const Model& v) {
    s << v.meshList();
    s << v.textureList();
    s << v.materialList();
    return s;
}

QDataStream &operator>>(QDataStream &s, Model& v) {
    s >> v.meshList();
    s >> v.textureList();
    s >> v.materialList();
    return s;
}

VOLCANO_RESOURCE_END
